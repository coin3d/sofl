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

#ifndef SOFL_SOFLTHUMBWHEEL_H
#define SOFL_SOFLTHUMBWHEEL_H

#include "Inventor/Fl/SoFlBasic.h"
#include "Inventor/Fl/widgets/SoAnyThumbWheel.h"

#include <FL/Fl_Window.H>

#include <string>
#include <Inventor/SbVec2s.h>

#include "Inventor/Fl/viewers/SoFlExaminerViewerP.h"

class SOFL_DLL_API SoFlThumbWheel : public Fl_Window {
public:
    enum Orientation { Horizontal, Vertical };

    explicit SoFlThumbWheel(Fl_Window * parent = 0, const char * name = 0);
    explicit SoFlThumbWheel(Orientation, Fl_Window * parent = 0, const char * name = 0);
    ~SoFlThumbWheel();

    void setOrientation(Orientation);
    Orientation orientation() const;

    void setValue(float value);
    float value() const;

    void setEnabled(bool enable);
    bool isEnabled() const;

    enum boundaryHandling {
        CLAMP,
        MODULATE,
        ACCUMULATE
    };
    void setRangeBoundaryHandling(boundaryHandling handling);
    boundaryHandling getRangeBoundaryHandling() const;

    SbVec2s sizeHint() const;

    void paintEvent(int );
    void mousePressEvent(int );
    void mouseReleaseEvent(int );
    void mouseMoveEvent(int );
    void mouseWheel(int );

private:
    void constructor(Orientation);
    void sendEvent(long id,
                   const std::string& event_id);

    SoFlThumbWheel(const SoFlThumbWheel & wheel);
    SoFlThumbWheel & operator = (const SoFlThumbWheel & wheel);

    enum State { Idle, Dragging, Disabled } state;

    Orientation orient;
    float wheelValue, tempWheelValue;
    int mouseDownPos, mouseLastPos;

    void initWheel(int diameter, int width);

    SoAnyThumbWheel * wheel;
    Fl_Pixmap**   pixmaps;
    int numPixmaps;
    int currentPixmap;



}; // class SoFlThumbWheel

// *************************************************************************


#endif //SOFL_SOFLTHUMBWHEEL_H
