#!/bin/bash

# --- Default Values ---
MODE="snapshot"
OUTPUT=""
APP_PATH=""
REFERENCE=""
DISPLAY_NUM=":99"
SCREEN_RES="1280x1024x24"

# --- Dependency Check ---
# Function to check if a command exists in the system
check_dependency() {
    if ! command -v "$1" &> /dev/null; then
        echo "Error: Required command '$1' is not installed."
        echo "Please install it using: sudo apt install $2"
        exit 1
    fi
}

# Check all necessary tools before starting
check_dependency "Xvfb" "xvfb"
check_dependency "scrot" "scrot"
check_dependency "ffmpeg" "ffmpeg"
check_dependency "compare" "imagemagick"

# --- Parse Arguments ---
for arg in "$@"; do
  case $arg in
    -video)
      MODE="video"
      shift
      ;;
    --output=*)
      OUTPUT="${arg#*=}"
      shift
      ;;
    -compare=*)
      REFERENCE="${arg#*=}"
      shift
      ;;
    *)
      # Assume it's the application path if not an option
      if [ -z "$APP_PATH" ]; then
        APP_PATH="$arg"
      fi
      ;;
  esac
done

# --- Validation ---

# 1. Check if app path is provided
if [ -z "$APP_PATH" ]; then
    echo "Usage: $0 [-video] [--output=filename] [-compare=reference_file] <path_to_app>"
    exit 1
fi

# 2. Check if the application executable actually exists
if [ ! -x "$APP_PATH" ]; then
    echo "Error: Application '$APP_PATH' not found or not executable."
    exit 1
fi

# 3. CRITICAL: Check if reference file exists if -compare is provided
if [ -n "$REFERENCE" ]; then
    if [ ! -f "$REFERENCE" ]; then
        echo "Error: Reference file '$REFERENCE' does not exist."
        exit 1
    fi
fi

# --- Setup Filenames (Output and Log) ---
TIMESTAMP=$(date +%Y%m%d_%H%M%S)
APP_NAME=$(basename "$APP_PATH")
LOG_FILE="/tmp/${APP_NAME}_${TIMESTAMP}.log"

if [ -z "$OUTPUT" ]; then
    RAND_STR=$RANDOM
    if [ "$MODE" == "video" ]; then
        OUTPUT="/tmp/test_${TIMESTAMP}_${RAND_STR}.mp4"
    else
        OUTPUT="/tmp/snapshot_${TIMESTAMP}_${RAND_STR}.png"
    fi
fi

echo "Mode: $MODE"
echo "Output: $OUTPUT"
echo "Log file: $LOG_FILE"

# --- Cleanup previous Xvfb locks and start server ---
# Extracts the number from :99 to find /tmp/.X99-lock
rm -f /tmp/.X${DISPLAY_NUM#:}-lock
Xvfb $DISPLAY_NUM -screen 0 $SCREEN_RES -ac &
XVFB_PID=$!
sleep 2

# --- Execution Logic ---
if [ "$MODE" == "video" ]; then
    # Start FFmpeg recording in background
    ffmpeg -y -f x11grab -video_size 1280x1024 -i ${DISPLAY_NUM}.0 \
           -codec:v libx264 -pix_fmt yuv420p -t 5 "$OUTPUT" &
    FFMPEG_PID=$!
    
    # Launch FLTK App with logging (stdout + stderr redirected)
    echo "--- App Log Started at $(date) ---" > "$LOG_FILE"
    WAYLAND_DISPLAY="" FLTK_BACKEND="x11" DISPLAY=$DISPLAY_NUM "$APP_PATH" >> "$LOG_FILE" 2>&1 &
    APP_PID=$!
    
    wait $FFMPEG_PID
else
    # Launch FLTK App with logging
    echo "--- App Log Started at $(date) ---" > "$LOG_FILE"
    WAYLAND_DISPLAY="" FLTK_BACKEND="x11" DISPLAY=$DISPLAY_NUM "$APP_PATH" >> "$LOG_FILE" 2>&1 &
    APP_PID=$!
    
    # Wait for UI to render then take a snapshot
    sleep 2
    DISPLAY=$DISPLAY_NUM scrot --overwrite "$OUTPUT"
fi

# --- Image Comparison Logic ---
# This part is only executed if REFERENCE is set (and we already know it exists)
if [ -n "$REFERENCE" ]; then
    echo "Comparing $OUTPUT with $REFERENCE..."
    
    # AE metric counts absolute pixel difference
    RAW_DIFF=$(compare -metric AE "$OUTPUT" "$REFERENCE" /tmp/diff_result.png 2>&1)
    
    # Extract the first integer (handles "15 (0.02)")
    DIFF_PIXELS=$(echo $RAW_DIFF | cut -d' ' -f1)

    if [[ "$DIFF_PIXELS" =~ ^[0-9]+$ ]]; then
        if [ "$DIFF_PIXELS" -eq 0 ]; then
            echo "SUCCESS: The images are identical."
        else
            echo "FAILURE: Found $DIFF_PIXELS different pixels."
            echo "Visual differences saved at /tmp/diff_result.png"
            # Optional: exit 1 here if you want the script to fail on mismatch
        fi
    else
        echo "Error during comparison command: $RAW_DIFF"
        kill $APP_PID $XVFB_PID 2>/dev/null
        exit 1
    fi
fi

# --- Cleanup processes ---
kill $APP_PID $XVFB_PID 2>/dev/null
echo "Process finished execution."
