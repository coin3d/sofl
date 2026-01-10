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

#include "Inventor/Fl/viewers/SoFlPlaneViewer.h"
#include "Inventor/Fl/viewers/SoFlPlaneViewerP.h"
#include "Inventor/Fl/common/pixmaps/ortho.xpm"
#include "Inventor/Fl/common/pixmaps/perspective.xpm"
#include "Inventor/Fl/common/pixmaps/x.xpm"
#include "Inventor/Fl/common/pixmaps/y.xpm"
#include "Inventor/Fl/common/pixmaps/z.xpm"
#include "Inventor/nodes/SoOrthographicCamera.h"
#include "Inventor/nodes/SoPerspectiveCamera.h"

#include "sofldefs.h"
#include "ButtonIndexValues.h"

#define PUBLIC(o) (o->pub)
#define PRIVATE(o) (o->pimpl)

Fl_Widget*
SoFlPlaneViewer::buildWidget(Fl_Widget* parent) {
    return inherited::buildWidget(parent);
}

void
SoFlPlaneViewer::createViewerButtons(Fl_Widget* parent,
                                     SbPList * buttons) {
    inherited::createViewerButtons(parent, buttons);
#if 0
    // add X, Y, Z viewpoint buttons
    PRIVATE(this)->buttons.x = new Fl_Button(parent, X_BUTTON, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);;
    PRIVATE(this)->buttons.x->SetBitmap(Fl_Image(x_xpm));
    buttons->append(PRIVATE(this)->buttons.x);

    PRIVATE(this)->buttons.y = new Fl_Button(parent, Y_BUTTON, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);;
    PRIVATE(this)->buttons.y->SetBitmap(Fl_Image(y_xpm));
    buttons->append(PRIVATE(this)->buttons.y);

    PRIVATE(this)->buttons.z =  new Fl_Button(parent, Z_BUTTON, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);;
    PRIVATE(this)->buttons.z->SetBitmap(Fl_Image(z_xpm));
    buttons->append(PRIVATE(this)->buttons.z);

    // add camera toggle button
    assert(PRIVATE(this)->pixmaps.perspective != NULL);
    assert(PRIVATE(this)->pixmaps.orthogonal != NULL);
    PRIVATE(this)->buttons.camera = new Fl_Button(parent, CAMERA_BUTTON, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);;

    Fl_Image * pixmap = NULL;
    SoType t = this->getCameraType();
    if (t.isDerivedFrom(SoOrthographicCamera::getClassTypeId()))
        pixmap = PRIVATE(this)->pixmaps.orthogonal;
    else if (t.isDerivedFrom(SoPerspectiveCamera::getClassTypeId()))
        pixmap = PRIVATE(this)->pixmaps.perspective;
    else assert(0 && "unsupported cameratype");

    PRIVATE(this)->buttons.camera->SetBitmap( wxBitmap(*pixmap));
    buttons->append(PRIVATE(this)->buttons.camera);
#endif

}

void
SoFlPlaneViewer::setCamera( SoCamera* camera) {
    if (camera) {
        SoType type = camera->getTypeId();
        SbBool orthogonal =
                type.isDerivedFrom(SoOrthographicCamera::getClassTypeId());
        const char * oldLabel = this->getRightWheelString();
        if (oldLabel) {
            if (orthogonal) {
                if (strcmp("Dolly",oldLabel) == 0)
                    this->setRightWheelString("Zoom");
            }
            else if (strcmp("Zoom",oldLabel) == 0)
                this->setRightWheelString("Dolly");
        }
        if (PRIVATE(this)->buttons.camera) {
#if 0
            PRIVATE(this)->buttons.camera->SetBitmap(
                    orthogonal ?
                    *(PRIVATE(this)->pixmaps.orthogonal) :
                    *(PRIVATE(this)->pixmaps.perspective));
#endif

        }
    }
    inherited::setCamera(camera);
}


SoFlPlaneViewerP::SoFlPlaneViewerP(SoFlPlaneViewer *v)
        : SoGuiPlaneViewerP(v){

}

void SoFlPlaneViewerP::constructor(SbBool buildnow) {
    this->commonConstructor(); // init generic stuff
#if 0
    this->pixmaps.orthogonal = new Fl_Image((const char **) ortho_xpm);
    this->pixmaps.perspective = new Fl_Image((const char **) perspective_xpm);

    PUBLIC(this)->setClassName("SoFlPlaneViewer");
    PUBLIC(this)->setLeftWheelString("transY");
    PUBLIC(this)->setBottomWheelString("transX");

    if (!buildnow) return;

    PUBLIC(this)->setSize(SbVec2s(550, 490)); // extra buttons -> more height
    Fl_Widget * viewer = PUBLIC(this)->buildWidget(PUBLIC(this)->getParentWidget());
    PUBLIC(this)->setBaseWidget(viewer);

    viewer->Bind(wxEVT_BUTTON,
                 &SoFlPlaneViewerP::cameraToggleClicked,
                 this,
                 CAMERA_BUTTON);
    viewer->Bind(wxEVT_BUTTON,
                 &SoFlPlaneViewerP::xClicked,
                 this,
                 X_BUTTON);
    viewer->Bind(wxEVT_BUTTON,
                 &SoFlPlaneViewerP::yClicked,
                 this,
                 Y_BUTTON);
    viewer->Bind(wxEVT_BUTTON,
                 &SoFlPlaneViewerP::zClicked,
                 this,
                 Z_BUTTON);
#endif

}

void SoFlPlaneViewerP::xClicked(int) {
    this->viewPlaneX();
}

void SoFlPlaneViewerP::yClicked(int) {
    this->viewPlaneY();
}

void SoFlPlaneViewerP::zClicked(int) {
    this->viewPlaneZ();
}

void SoFlPlaneViewerP::cameraToggleClicked(int) {
    PUBLIC(this)->toggleCameraType();
}

#undef PUBLIC
#undef PRIVATE
