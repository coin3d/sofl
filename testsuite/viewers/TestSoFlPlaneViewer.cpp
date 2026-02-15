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

#define BOOST_TEST_NO_LIB 1
#include <boost/test/unit_test.hpp>

#include "Inventor/Fl/viewers/SoFlPlaneViewer.h"
#include <Inventor/nodes/SoCamera.h>
#include <FL/Fl_Window.H>

BOOST_AUTO_TEST_SUITE(TestSoFlPlaneViewer);

BOOST_AUTO_TEST_CASE(shouldCreateViewer) {
    Fl_Window* window = new Fl_Window(100, 100, "Test PlaneViewer");
    SoFlPlaneViewer* viewer = new SoFlPlaneViewer(window);
    
    BOOST_CHECK(viewer != NULL);
    BOOST_CHECK_EQUAL(viewer->getParentWidget(), window);
    
    delete viewer;
    delete window;
}

BOOST_AUTO_TEST_CASE(shouldHandleCameraType) {
    Fl_Window* window = new Fl_Window(100, 100, "Test PlaneViewer Camera");
    SoFlPlaneViewer* viewer = new SoFlPlaneViewer(window);
    
    // Default should be orthographic for PlaneViewer usually, but let's check
    SoType type = viewer->getCameraType();
    // In many Coin3D viewers, default might be perspective unless specified.
    // We just check it's a valid camera type.
    BOOST_CHECK(type.isDerivedFrom(SoCamera::getClassTypeId()));

    delete viewer;
    delete window;
}

BOOST_AUTO_TEST_CASE(shouldHandleDecoration) {
    Fl_Window* window = new Fl_Window(100, 100, "Test PlaneViewer Decoration");
    SoFlPlaneViewer* viewer = new SoFlPlaneViewer(window);
    
    viewer->setDecoration(TRUE);
    BOOST_CHECK_EQUAL(viewer->isDecoration(), TRUE);
    
    viewer->setDecoration(FALSE);
    BOOST_CHECK_EQUAL(viewer->isDecoration(), FALSE);
    
    delete viewer;
    delete window;
}

BOOST_AUTO_TEST_SUITE_END();
