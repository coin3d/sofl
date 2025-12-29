# Use a recent Ubuntu base image
FROM ubuntu:22.04

# Evita interazioni durante l'installazione dei pacchetti
ENV DEBIAN_FRONTEND=noninteractive

# Installa le dipendenze di sistema, compilatore e librerie grafiche
RUN apt-get update && apt-get install -y \
    git \
    build-essential \
    cmake \
    pkg-config \
    libglu1-mesa-dev \
    freeglut3-dev \
    libglew-dev \
    libx11-dev \
    libxext-dev \
    libxft-dev \
    libxinerama-dev \
    libxcursor-dev \
    libboost-all-dev \
    && rm -rf /var/lib/apt/lists/*


RUN git clone --depth 1 --branch v4.0.6 https://github.com/coin3d/coin.git /tmp/coin_source_dir
RUN cmake -S /tmp/coin_source_dir -B /tmp/coin_source_dir/coin_build_dir -DCOIN_BUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_STANDARD=11
RUN cmake --build /tmp/coin_source_dir/coin_build_dir --target install --config Release -- -j8
RUN git clone --depth 1 --branch release-1.4.4 https://github.com/fltk/fltk /tmp/fltk_source_dir
RUN cmake -S /tmp/fltk_source_dir -B /tmp/fltk_source_dir/fltk_build_dir -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_STANDARD=11 -DFLTK_BUILD_SHARED_LIBS=ON
RUN cmake --build /tmp/fltk_source_dir/fltk_build_dir --target install --config Release -- -j8

# Imposta la directory di lavoro
WORKDIR /app

# Copia i file del progetto nel container
COPY . .

# Crea una directory di build
RUN ls -latr && \
    cmake -S . -B cmake_build_dir -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=cmake_install_dir && \
    cmake --build cmake_build_dir --target install --config Release -- -j4

# Comando di default (puoi cambiarlo per eseguire i test)
# CMD ["ctest", "--test-dir", "build", "--output-on-failure"]