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

#include <FL/Fl_Group.H>
#include <FL/Fl_RGB_Image.H>

#include "Inventor/Fl/SoFlBasic.h"
#include "Inventor/Fl/widgets/SoAnyThumbWheel.h"

#include <Inventor/SbVec2s.h>

class SOFL_DLL_API SoFlThumbWheel : public Fl_Group {
public:
    enum Orientation { Horizontal, Vertical };

    SoFlThumbWheel(const SoFlThumbWheel & wheel) = delete;
    SoFlThumbWheel & operator = (const SoFlThumbWheel & wheel) = delete;

    explicit SoFlThumbWheel(const SbVec2s& pos, const char * name = nullptr);
    explicit SoFlThumbWheel(Orientation, const SbVec2s& pos, const char * name = nullptr);
    ~SoFlThumbWheel() override;

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

private:

    enum State { Idle, Dragging, Disabled } state {Disabled};

    Orientation orient{Horizontal};
    float wheelValue{}, tempWheelValue{};
    int mouseDownPos{}, mouseLastPos{};
    SoAnyThumbWheel * wheel{};
    uint8_t** pixmaps{};
    Fl_RGB_Image** fl_images{};
    int numPixmaps{};
    int currentPixmap{};
    SbVec2s th_position;

    void constructor(Orientation);
    void initWheel(int diameter, int width);
    void cleanPixmaps();
    void mousePressEvent(int );
    void mouseReleaseEvent(int );
    void mouseMoveEvent(int );
    // TODO: void mouseWheel(int);

    void draw() override;
    int handle(int event) override;
};

#endif //SOFL_SOFLTHUMBWHEEL_H
