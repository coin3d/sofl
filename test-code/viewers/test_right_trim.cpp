#include <Inventor/Fl/SoFl.h>
#define protected public
#include <Inventor/Fl/viewers/SoFlFullViewer.h>
#undef protected
#include <FL/Fl_Window.H>
#include <iostream>
#include <cassert>

int main() {
    Fl_Window* window = SoFl::init("test_right_trim");

    SoFlFullViewer* viewer =
        new SoFlFullViewer(window,
                           "Test Right Trim",
                           FALSE,
                           SoFlFullViewer::BUILD_NONE,
                           SoFlViewer::BROWSER,
                           FALSE);

    // Inizialmente rightWheel dovrebbe essere null se BUILD_NONE è usato
    // o comunque vogliamo testare buildRightTrim specificatamente.
    viewer->rightWheel = nullptr;
    viewer->leftWheel = nullptr;

    std::cout << "Calling buildRightTrim..." << std::endl;
    viewer->buildRightTrim(window);

    if (viewer->rightWheel != nullptr) {
        std::cout << "SUCCESS: rightWheel is initialized." << std::endl;
    } else {
        std::cerr << "FAILURE: rightWheel is NOT initialized!" << std::endl;
    }

    if (viewer->leftWheel != nullptr) {
        std::cerr << "ERROR: leftWheel was initialized instead of rightWheel!" << std::endl;
    }

    viewer->rightWheel = nullptr;
    viewer->leftWheel = nullptr;
    std::cout << "Calling buildLeftTrim..." << std::endl;
    viewer->buildLeftTrim(window);

    if (viewer->leftWheel != nullptr) {
        std::cout << "SUCCESS: leftWheel is initialized." << std::endl;
    } else {
        std::cerr << "FAILURE: leftWheel is NOT initialized!" << std::endl;
    }

    if (viewer->rightWheel != nullptr) {
        std::cerr << "ERROR: rightWheel was initialized instead of leftWheel!" << std::endl;
    }

    delete viewer;
    return (viewer->rightWheel == nullptr && viewer->leftWheel != nullptr) ? 0 : 1;
}
