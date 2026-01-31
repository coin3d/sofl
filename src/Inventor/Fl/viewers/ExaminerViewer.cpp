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

#include "Inventor/Fl/viewers/SoFlExaminerViewer.h"
#include "Inventor/Fl/viewers/SoFlExaminerViewerP.h"
#include "Inventor/Fl/viewers/SoFlViewer.h"
#include "Inventor/Fl/viewers/SoFlFullViewer.h"
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>

#include "sofldefs.h"

#define PRIVATE(obj) ((obj)->pimpl)
#define PUBLIC(obj) ((obj)->pub)

SOFL_OBJECT_SOURCE(SoFlExaminerViewer);

namespace {
void onCameraToggleClicked(Fl_Widget *, void *data) {
    auto *pimpl = static_cast<SoFlExaminerViewerP *>(data);
    if (pimpl) pimpl->cameratoggleClicked(0);
}
} // namespace

SoFlExaminerViewer::SoFlExaminerViewer(Fl_Window* parent,
                                       char const* name,
                                       int embed,
                                       SoFlFullViewer::BuildFlag flag,
                                       SoFlViewer::Type type)
        : inherited(parent, name, embed, flag, type, FALSE) {
    PRIVATE(this) = new SoFlExaminerViewerP(this);
    PRIVATE(this)->constructor(TRUE);
}

SoFlExaminerViewer::SoFlExaminerViewer(Fl_Window *parent,
                                       const char *name,
                                       SbBool embed,
                                       SoFlFullViewer::BuildFlag flag,
                                       SoFlViewer::Type type,
                                       SbBool build)
    : inherited(parent, name, embed, flag, type, build) {
    PRIVATE(this) = new SoFlExaminerViewerP(this);
    PRIVATE(this)->constructor(build);
}

SoFlExaminerViewer::~SoFlExaminerViewer() {
    delete PRIVATE(this);
}

void
SoFlExaminerViewer::setCamera(SoCamera * newCamera) {
    // This method overridden from parent class to toggle the camera
    // type selection button pixmap and string of the zoom/dolly
    // thumbwheel.

    if (newCamera) {
        SoType camtype = newCamera->getTypeId();
        SbBool orthogonal =
                camtype.isDerivedFrom(SoOrthographicCamera::getClassTypeId());

        const char * oldLabel = this->getRightWheelString();
        if (oldLabel) {
            if (orthogonal) {
                if (strcmp("Dolly",oldLabel) == 0)
                    this->setRightWheelString("Zoom");
            }
            else if (strcmp("Zoom",oldLabel) == 0)
                this->setRightWheelString("Dolly");
        }
        if (PRIVATE(this)->cameratogglebutton) {
            PRIVATE(this)->cameratogglebutton->image(
                    orthogonal ?
                    PRIVATE(this)->orthopixmap :
                    PRIVATE(this)->perspectivepixmap);
            PRIVATE(this)->cameratogglebutton->redraw();
        }
    }

    inherited::setCamera(newCamera);
}

void
SoFlExaminerViewer::createViewerButtons(Fl_Window* parent,
                                        SbPList * buttonlist) {

    inherited::createViewerButtons(parent, buttonlist);
    PRIVATE(this)->cameratogglebutton = new Fl_Button(0,0,10,10,nullptr);
    PRIVATE(this)->cameratogglebutton->copy_label("CAMERA");
    PRIVATE(this)->cameratogglebutton->callback(&onCameraToggleClicked,
                                                PRIVATE(this));

    Fl_Image * p = nullptr;
    SoType t = this->getCameraType();
    if (t.isDerivedFrom(SoOrthographicCamera::getClassTypeId()))
        p = PRIVATE(this)->orthopixmap;
    else if (t.isDerivedFrom(SoPerspectiveCamera::getClassTypeId()))
        p = PRIVATE(this)->perspectivepixmap;
    else assert(0 && "unsupported cameratype");

    PRIVATE(this)->cameratogglebutton->image(p);
    PRIVATE(this)->cameratogglebutton->redraw();
    buttonlist->append(PRIVATE(this)->cameratogglebutton);
}
