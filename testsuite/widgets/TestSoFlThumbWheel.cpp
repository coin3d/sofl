/**************************************************************************\
* BSD 3-Clause License
 *
 * Copyright (c) 2025, Fabrizio Morciano
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

#include <boost/test/unit_test.hpp>

#include <Inventor/Fl/SoFl.h>
#define protected public
#define private public
#include <Inventor/Fl/viewers/SoFlFullViewer.h>
#include <Inventor/Fl/widgets/SoFlThumbWheel.h>
#undef protected
#undef private
#include <FL/Fl_Window.H>

namespace
{
    constexpr int THUMBWHEEL_WIDTH = 122;
    constexpr int THUMBWHEEL_HEIGHT = 24;
    constexpr int THUMBWHEEL_PIXMAP_WIDTH = 116;
    constexpr int THUMBWHEEL_PIXMAP_HEIGHT = 12;
    constexpr int WINDOW_WIDTH = 100;
    constexpr int WINDOW_HEIGHT = 100;


}

BOOST_AUTO_TEST_SUITE(TestSoFlThumbWheel)

BOOST_AUTO_TEST_CASE(test_wheels_initialization) {
    auto window = new Fl_Window(WINDOW_WIDTH, WINDOW_HEIGHT,
        "test_wheels_initialization");

    auto viewer =
        new SoFlFullViewer(window,
                           "Test Wheels",
                           FALSE,
                           SoFlFullViewer::BUILD_NONE,
                           SoFlViewer::BROWSER,
                           FALSE);

    viewer->rightWheel = nullptr;
    viewer->leftWheel = nullptr;
    viewer->bottomWheel = nullptr;

    // Test Right Trim
    viewer->buildRightTrim(window);
    BOOST_CHECK(viewer->rightWheel != nullptr);
    BOOST_CHECK(viewer->rightWheelVal == dynamic_cast<SoFlThumbWheel*>(viewer->rightWheel)->value());
    BOOST_CHECK(viewer->leftWheel == nullptr);
    BOOST_CHECK(viewer->bottomWheel == nullptr);

    viewer->rightWheel = nullptr;
    viewer->leftWheel = nullptr;
    viewer->bottomWheel = nullptr;

    // Test Left Trim
    viewer->buildLeftTrim(window);
    BOOST_CHECK(viewer->leftWheel != nullptr);
    BOOST_CHECK(viewer->leftWheelVal == dynamic_cast<SoFlThumbWheel*>(viewer->leftWheel)->value());
    BOOST_CHECK(viewer->rightWheel == nullptr);
    BOOST_CHECK(viewer->bottomWheel == nullptr);

    viewer->rightWheel = nullptr;
    viewer->leftWheel = nullptr;
    viewer->bottomWheel = nullptr;

    // Test Bottom Trim
    viewer->buildBottomTrim(window);
    BOOST_CHECK(viewer->bottomWheel != nullptr);
    BOOST_CHECK(viewer->bottomWheelVal == dynamic_cast<SoFlThumbWheel*>(viewer->bottomWheel)->value());
    BOOST_CHECK(viewer->rightWheel == nullptr);
    BOOST_CHECK(viewer->leftWheel == nullptr);

    delete viewer;
    delete window;
}

BOOST_AUTO_TEST_CASE(test_wheels_size_horizontal)
{
    auto window = new Fl_Window(WINDOW_WIDTH, WINDOW_HEIGHT,
        "test_wheels_size");
    auto wheel = new SoFlThumbWheel(SoFlThumbWheel::Horizontal, SbVec2s(10,10));
    BOOST_CHECK(wheel->sizeHint() == SbVec2s(THUMBWHEEL_WIDTH, THUMBWHEEL_HEIGHT));
    delete wheel;
    delete window;
}

BOOST_AUTO_TEST_CASE(test_wheels_size_vertical)
{
    auto window = new Fl_Window(THUMBWHEEL_WIDTH, THUMBWHEEL_HEIGHT, "test_wheels_size");
    auto wheel = new SoFlThumbWheel(SoFlThumbWheel::Vertical, SbVec2s(10,10));
    BOOST_CHECK(wheel->sizeHint() == SbVec2s(THUMBWHEEL_HEIGHT, THUMBWHEEL_WIDTH ));
    delete wheel;
    delete window;
}

BOOST_AUTO_TEST_CASE(test_wheels_getPosition_vertical)
{
    auto window = new Fl_Window(THUMBWHEEL_WIDTH, THUMBWHEEL_HEIGHT, "test_wheels_size");
    auto wheel = new SoFlThumbWheel(SoFlThumbWheel::Vertical, SbVec2s(10,10));
    wheel->initWheel(THUMBWHEEL_PIXMAP_WIDTH,THUMBWHEEL_PIXMAP_HEIGHT);
    BOOST_CHECK(wheel->numPixmaps > 0);
    BOOST_CHECK(wheel->pixmaps != nullptr);
    BOOST_CHECK(wheel->fl_images != nullptr);
    int a_pixmap = 0;
    auto rgb_image = wheel->fl_images[0];
    BOOST_CHECK(rgb_image != nullptr);
    BOOST_CHECK(wheel->getPosition(rgb_image) == SbVec2s(6, 3 ));
    delete wheel;
    delete window;
}

BOOST_AUTO_TEST_CASE(test_wheels_getPosition_horizontal)
{
    auto window = new Fl_Window(THUMBWHEEL_WIDTH, THUMBWHEEL_HEIGHT, "test_wheels_size");
    auto wheel = new SoFlThumbWheel(SoFlThumbWheel::Horizontal, SbVec2s(10,10));
    wheel->initWheel(THUMBWHEEL_PIXMAP_WIDTH,THUMBWHEEL_PIXMAP_HEIGHT);
    BOOST_CHECK(wheel->numPixmaps > 0);
    BOOST_CHECK(wheel->pixmaps != nullptr);
    BOOST_CHECK(wheel->fl_images != nullptr);
    int a_pixmap = 0;
    auto rgb_image = wheel->fl_images[0];
    BOOST_CHECK(rgb_image != nullptr);
    auto val = wheel->getPosition(rgb_image);
    BOOST_CHECK(wheel->getPosition(rgb_image) == SbVec2s(3, 6 ));
    delete wheel;
    delete window;
}



BOOST_AUTO_TEST_SUITE_END()
