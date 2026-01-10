/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

/*
  This is just a simple test application to try out the functionality
  of the SoFlThumbWheel widget.

  FIXME: this doesn't work at the moment (the SoFlThumbWheel API is
  not exported in the SoFl library). 20001125 mortene.
*/


#include <iostream>
#include <ostream>
#include <Inventor/Fl/widgets/SoFlThumbWheel.h>
#include <Inventor/Fl/SoFl.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>

// *************************************************************************

int
main(
    int /*argc*/,
    char** argv)
{
    // 1. Finestra Principale
    Fl_Window* main_win = new Fl_Window(400, 300, "Finestra Principale");

    //Fl_Window* main_win =  SoFl::init(argv[0]);
    main_win->size(800, 800);
    main_win->begin();

    //*
    auto wheel_h = new SoFlThumbWheel(SoFlThumbWheel::Horizontal, SbVec2s(24, 0));
    wheel_h->color(FL_GREEN);
    //*/
//*
    auto wheel_v = new SoFlThumbWheel(SoFlThumbWheel::Vertical, SbVec2s(0, 24));
    wheel_v->color(FL_RED);
    main_win->end();
//*/
    std::clog << "main_win->children():" << main_win->children() << std::endl;
    assert(main_win->children() > 0);

    main_win->show();

    return Fl::run();;
}
