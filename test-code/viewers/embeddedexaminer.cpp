/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

/*
  Demonstrates embedding of an SoFlExaminerViewer within a simple
  widget hierarchy.
*/

/***********************************************************************/

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Tile.H>
#include <FL/Fl_Box.H>

#include <Inventor/Fl/SoFl.h>
#include <Inventor/Fl/viewers/SoFlExaminerViewer.h>
#include <Inventor/nodes/SoCone.h>

#include "common/SceneBuilder.h"

int main(int argc, char **argv) {

    Fl_Window* window = SoFl::init("SoFl Split Example");
    window->size(800, 600);

    Fl_Tile* tile = new Fl_Tile(0, 0, 800, 600);

    Fl_Box* leftBox = new Fl_Box(0, 0, 200, 600, "Left panel");
    leftBox->box(FL_FLAT_BOX);
    leftBox->color(FL_DARK2);
    leftBox->labelsize(14);
    
    SoFlExaminerViewer* viewer = new SoFlExaminerViewer(window);
    viewer->setSize(SbVec2s(600,600));
    viewer->getGLWidget()->position(200,0);
    auto cameraAndCone = buildCameraAndCone();
    cameraAndCone.first->viewAll(cameraAndCone.second,viewer->getViewportRegion());

    viewer->setSceneGraph(cameraAndCone.second);

    tile->end();

    window->resizable(tile);
    window->show(argc, argv);
    viewer->show();

    SoFl::mainLoop();

    return 0;
}

