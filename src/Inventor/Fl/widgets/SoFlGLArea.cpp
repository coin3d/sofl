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

#include "Inventor/Fl/widgets/SoFlGLArea.h"
#include "Inventor/Fl/SoFlGLWidgetP.h"
#include <Inventor/errors/SoDebugError.h>

#include "sofldefs.h"


SoFlGLArea::SoFlGLArea(Fl_Widget *parent,
                       SoFlGLWidgetP *parentW,
                       Fl_Mode attributes)
    : Fl_Gl_Window(parent->x(),
                   parent->y(),
                   parent->w(),
                   parent->h())
, widget_p(parentW)
, gl_format(attributes){
    mode(gl_format);
    this->copy_label("SoFlGLArea");
    gl_real_context = nullptr;
    is_gl_initialized = false;
}

SoFlGLArea::~SoFlGLArea() {
    SOFL_STUB();
}

void SoFlGLArea::draw() {
    SOFL_STUB();
#if SOFL_DEBUG
    SoDebugError::postInfo("SoFlGLArea::draw",
                           "size:%d %d",
                           w(),
                           h());
#endif

    if (!valid()) {
        InitGL();
        // ...other initialization...
#if SOFL_DEBUG
        SoDebugError::postInfo("SoFlGLArea::draw",
                               "%s",
                               "!valid()");
#endif
    }

    if (!context_valid()) {
        // ...load textures, etc. ...
#if SOFL_DEBUG
        SoDebugError::postInfo("SoFlGLArea::draw",
                               "%s",
                               "!context_valid()");
#endif
    }
#if SOFL_DEBUG
    SoDebugError::postInfo("SoFlGLArea::draw",
                           "%s",
                           "DRAW!!!!");
#endif

    widget_p->concreteRedraw();

    Fl_Gl_Window::draw(); // Draw FLTK child widgets.
}

int SoFlGLArea::handle(int event) {
#if SOFL_DEBUG && 0
    SoDebugError::postInfo("SoFlGLArea::handle",
                           "event: %d",
                           event);
#endif
    switch (event) {
        case FL_KEYBOARD:
            widget_p->onKey(event);
            return 1;
        case FL_PUSH:
        case FL_RELEASE:
        case FL_DRAG:
        case FL_MOVE:
            widget_p->onMouse(event);
            return 1;
        case FL_ENTER:
        case FL_LEAVE:
            return 1;
        default:
            break;
    }
    return Fl_Gl_Window::handle(event); // pass other events to base class
}

void SoFlGLArea::InitGL() {
    if (!is_gl_initialized) {
        is_gl_initialized = true;
        this->make_current();
        gl_real_context = this->context();
        assert(gl_real_context != nullptr);
        widget_p->initGL();
    }
}

void SoFlGLArea::makeCurrent() {
    this->make_current();
}
