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

#ifndef SOFL_SOFLGLAREA_H
#define SOFL_SOFLGLAREA_H

#include <FL/Fl_Gl_Window.H>
#include <FL/Enumerations.H>

class SoFlGLWidgetP;

class SoFlGLArea : public Fl_Gl_Window {
public:

    SoFlGLArea(Fl_Widget *parent,
               SoFlGLWidgetP *parentWidget,
               Fl_Mode );

    virtual ~SoFlGLArea();

    void makeCurrent();

    static bool isGLFeatureAvailable(Fl_Mode &,
                                     Fl_Mode feature);

    static bool areEqual(Fl_Mode &format1,
                         Fl_Mode &format2);

protected:
    int handle(int event) override;

    void draw() override;

private:
    void InitGL();

    SoFlGLWidgetP* widget_p;
    GLContext gl_real_context;
    bool is_gl_initialized;
    Fl_Mode gl_format;
};


#endif //SOFL_SOFLGLAREA_H
