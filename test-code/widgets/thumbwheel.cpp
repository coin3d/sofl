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


#include <Inventor/Fl/widgets/SoFlThumbWheel.h>
#include <Inventor/Fl/SoFl.h>
#include <FL/Fl.H>

// *************************************************************************

int
main(
    int /*argc*/,
    char** argv)
{
    Fl_Window* window = SoFl::init(argv[0]);
    window->size(800,800);
    auto wheel_h = new SoFlThumbWheel(SoFlThumbWheel::Horizontal, window);

    wheel_h->position(10,10);
    wheel_h->size(wheel_h->sizeHint()[0], wheel_h->sizeHint()[1]);
    wheel_h->color(FL_GREEN);

    auto wheel_v = new SoFlThumbWheel(SoFlThumbWheel::Vertical, window);
    wheel_v->position(200,0);
    wheel_v->size(wheel_v->sizeHint()[0], wheel_v->sizeHint()[1]);

    wheel_v->color(FL_RED);

    wheel_h->show();
    wheel_v->show();
    window->end();
    window->show();
    Fl::run();
    return true;
}
