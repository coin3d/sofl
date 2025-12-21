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
#include "fl/fl.h"

#include "common/SimpleFrame.h"

// Define a new application type
class MyApp : public wxApp
{
public:
    virtual bool OnInit() wxOVERRIDE {
        if ( !wxApp::OnInit() )
            return false;

        SimpleFrame *aFrame = new SimpleFrame(NULL,
                                              "thumb wheel",
                                              wxDefaultPosition,
                                              wxSize(300,300));

        wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
        SoFlThumbWheel *wheel_h = new SoFlThumbWheel(SoFlThumbWheel::Horizontal,
                                                     aFrame);

        wheel_h->SetSize(wheel_h->sizeHint());
        //wheel_h->SetMinSize( wheel_h->sizeHint());

        sizer->Add(wheel_h, 1, wxALL| wxEXPAND, 5 );
        SoFlThumbWheel *wheel_v = new SoFlThumbWheel(SoFlThumbWheel::Vertical,
                                                     aFrame);
        // wheel_v->SetSize(30, 200);
        wheel_v->SetSize(wheel_v->sizeHint());
        //wheel_h->SetPosition(wxPoint(40, 100));
        wheel_v->SetBackgroundColour(wxColour(255,0,0));
        wheel_h->SetBackgroundColour(wxColour(0,255,0));
        sizer->Add(wheel_v, 1, wxALL| wxEXPAND, 5 );
        aFrame->SetSizer(sizer);
        wheel_h->Show();
        wheel_v->Show();
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
