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

//
// Tiny OpenGL v3 demo program for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2024 by Bill Spitzak and others.
//
// This library is free software. Distribution and use rights are outlined in
// the file "COPYING" which should have been included with this file.  If this
// file is missing or damaged, see the license at:
//
//     https://www.fltk.org/COPYING.php
//
// Please see the following page on how to report bugs and issues:
//
//     https://www.fltk.org/bugs.php
//

#include <stdarg.h>
#include <FL/Fl.H>
#include <FL/platform.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>

#include <Inventor/Fl/SoFl.h>
#include <Inventor/Fl/SoFlRenderArea.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoSeparator.h>

void add_output(const char *format, ...);

SoFlRenderArea *
buildFlRenderArea(Fl_Window *parent, int x, int y, int w, int h) {
    // Initialize Inventor.
    SoFl::init(parent); // pass the app name

    // Make a scene containing a red cone
    SoSeparator *root = new SoSeparator;
    SoPerspectiveCamera *myCamera = new SoPerspectiveCamera;
    SoMaterial *myMaterial = new SoMaterial;
    root->ref();
    root->addChild(myCamera);
    root->addChild(new SoDirectionalLight);
    myMaterial->diffuseColor.setValue(1.0, 0.0, 0.0); // Red
    root->addChild(myMaterial);
    root->addChild(new SoCone);

    // Create a renderArea in which to see our scene graph.
    // The render area will appear within the main window.
    auto myRenderArea = new SoFlRenderArea(parent, nullptr, TRUE);
    myRenderArea->getGLWidget()->position(x,y);
    myRenderArea->setSize(SbVec2s(w,h));

    // Make myCamera see everything.
    myCamera->viewAll(root, myRenderArea->getViewportRegion());

    // Put our scene in myRenderArea, change the title
    myRenderArea->setSceneGraph(root);

    return myRenderArea;
}

void toggle_double(Fl_Widget *wid, void *data) {
    static bool doublebuff = true;
    doublebuff = !doublebuff;

    Fl_Gl_Window *glwin = (Fl_Gl_Window*)data;
    add_output("push  Fl_Gl_Window::pixels_per_unit()=%.1f\n", glwin->pixels_per_unit());
    /*
    int flags = glwin->mode();
    if (doublebuff) flags |= FL_DOUBLE; else flags &= ~FL_DOUBLE;
    glwin->reset();
    glwin->hide();
    glwin->mode(flags);
    glwin->show();
    //*/
}


Fl_Text_Display *output; // shared between output_win() and add_output()

void output_win(Fl_Window *gl)
{
  output = new Fl_Text_Display(300,0,500, 280);
  Fl_Light_Button *lb = new Fl_Light_Button(300, 280, 500, 20, "Double-Buffered");
  lb->callback(toggle_double);
  lb->user_data(gl);
  lb->value(1);
  output->buffer(new Fl_Text_Buffer());
}


void add_output(const char *format, ...) {
    va_list args;
    char line_buffer[10000];
    va_start(args, format);
    vsnprintf(line_buffer, sizeof(line_buffer) - 1, format, args);
    va_end(args);
    output->buffer()->append(line_buffer);
    output->scroll(10000, 0);
    output->redraw();
}


void button_cb(Fl_Widget *widget, void *) {
    add_output("run %s callback\n", widget->label());
}

void add_widgets(Fl_Window *g) {
    Fl::set_color(FL_FREE_COLOR, 255, 255, 255, 140); // partially transparent white
    g->begin();
    // Create here widgets to go above the GL3 scene
    Fl_Button *b = new Fl_Button(0, 0, 60, 30, "button");
    b->color(FL_FREE_COLOR);
    b->box(FL_DOWN_BOX);
    b->callback(button_cb, NULL);
    Fl_Button *b2 = new Fl_Button(0, 170, 60, 30, "button2");
    b2->color(FL_FREE_COLOR);
    b2->box(FL_BORDER_BOX);
    b2->callback(button_cb, NULL);
    g->end();
}


int main(int argc, char **argv) {
    Fl::use_high_res_GL(1);
    Fl_Window *topwin = new Fl_Window(800, 300);
    auto *win = buildFlRenderArea(topwin,0, 0, 300, 300);
    win->getGLWidget()->end();
    output_win(win->getGLWidget());
    add_widgets(win->getGLWidget());
    topwin->end();
    topwin->resizable(win->getGLWidget());
    topwin->copy_label("Click GL panel to reshape");
    topwin->show(argc, argv);
    Fl::run();
}
