/**************************************************************************\
 * BSD 3-Clause License
 *
 * Copyright (c) 2022, Fabrizio Morciano
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

#ifndef SOFL_SOFLPLANEVIEWERP_H
#define SOFL_SOFLPLANEVIEWERP_H

#ifndef SOFL_INTERNAL
#error this is a private header file
#endif

#include <Inventor/SbBasic.h>
#include "Inventor/Fl/viewers/SoGuiPlaneViewerP.h"
#include <FL/Fl_Button.H>

class SoFlPlaneViewerP : public SoGuiPlaneViewerP {
public:
    explicit SoFlPlaneViewerP(SoFlPlaneViewer*);
    void constructor(SbBool buildnow);

    struct {
        Fl_Button * x, * y, * z;
        Fl_Button * camera;
    } buttons;

    struct {
        Fl_Image * orthogonal, * perspective;
    } pixmaps;

    void xClicked(int);
    void yClicked(int);
    void zClicked(int);
    void cameraToggleClicked(int);
};


#endif //SOFL_SOFLPLANEVIEWERP_H
