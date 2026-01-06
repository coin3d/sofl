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

#include "Inventor/Fl/viewers/SoFlExaminerViewerP.h"
#include "Inventor/Fl/viewers/SoFlExaminerViewer.h"
#include "Inventor/Fl/common/pixmaps/ortho.xpm"
#include "Inventor/Fl/common/pixmaps/perspective.xpm"
#include "ButtonIndexValues.h"

#include <FL/Fl_XPM_Image.H>

#define PRIVATE(obj) ((obj)->pimpl)
#define PUBLIC(obj) ((obj)->pub)

SoFlExaminerViewerP::SoFlExaminerViewerP(SoFlExaminerViewer *publ)
        : SoGuiExaminerViewerP(publ) {

}

SoFlExaminerViewerP::~SoFlExaminerViewerP() {
    this->genericDestructor();
}

void
SoFlExaminerViewerP::constructor(const SbBool build) {
    this->genericConstructor();

    this->cameratogglebutton = nullptr;
    delete this->orthopixmap;
    this->orthopixmap = new Fl_Pixmap(const_cast<char**>(ortho_xpm));
    delete this->perspectivepixmap;
    this->perspectivepixmap = new Fl_Pixmap(const_cast<char**>( perspective_xpm));
    assert(this->orthopixmap->w() == this->perspectivepixmap->h());
    assert(this->orthopixmap->w() == this->perspectivepixmap->h());

    PUBLIC(this)->setClassName("SoFlExaminerViewer");
    PUBLIC(this)->setPopupMenuString("Examiner Viewer");
    PUBLIC(this)->setLeftWheelString("RotX");
    PUBLIC(this)->setBottomWheelString("RotY");

    if (build) {
        Fl_Window *widget = PUBLIC(this)->buildWidget(PUBLIC(this)->getParentWidget());
#if 0
        widget->Bind( wxEVT_BUTTON,
                      &SoFlExaminerViewerP::cameratoggleClicked,
                      this,
                      CAMERA_BUTTON);
        PUBLIC(this)->setBaseWidget(widget);
        widget->SetMinSize(wxSize(500,300));
#   endif
        PUBLIC(this)->setSize(SbVec2s(500, 300));
    }
}

void
SoFlExaminerViewerP::cameratoggleClicked(int) {
    if (PUBLIC(this)->getCamera())
        PUBLIC(this)->toggleCameraType();
}


