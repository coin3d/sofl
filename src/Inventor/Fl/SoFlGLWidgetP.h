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
#ifndef SOFL_SOFLGLWIDGETP_H
#define SOFL_SOFLGLWIDGETP_H

#include "Inventor/Fl/SoGuiGLWidgetP.h"
#include "Inventor/Fl/SoFlGLWidget.h"

#include <Inventor/SbVec2s.h>

#include <FL/Fl_Widget.H>

#include <set>
#include <vector>

class SoFlGLArea;

class SoFlGLWidgetP :  public SoGuiGLWidgetP
{
public:

    explicit SoFlGLWidgetP(SoFlGLWidget * publ);

    virtual ~SoFlGLWidgetP();

    void initGLModes(int);

    std::vector<int> gl_attributes;
    SoFlGLArea* buildGLWidget();

    SbVec2s glSize;
    SbVec2s glSizeUnscaled;
    SbBool wasresized;

    Fl_Widget * currentglwidget;
    Fl_Widget * previousglwidget;
    SoFlGLArea * currentglarea;
    SoFlGLArea * previousglarea;
    Fl_Widget * glparent;

    int borderthickness;

    const GLContext * oldcontext;

    void gl_init(int);
    void gl_reshape(int);
    void gl_exposed(int);
    void onMouse(int);
    void onKey(int);

    static bool isAPanel(Fl_Widget*);
    void addSizer();

    bool hasZBuffer() const;
    bool hasOverlay() const;

    /*
    // Required by the common code
    static void eventHandler(Fl_Widget*, void*, int, bool*);
*/
protected:
    virtual SbBool isDirectRendering(void);

    /*
    const GLContext *getOverlayContext(void);
    const GLContext *getNormalContext(void);
    */
};


#endif //SOFL_SOFLGLWIDGETP_H
