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
  Demonstrates embedding of an SoFlExaminerViewer within a simple
  widget hierarchy.
*/

/***********************************************************************/

#include "Inventor/Fl/SoFl.h"
#include "Inventor/Fl/viewers/SoFlExaminerViewer.h"
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoSeparator.h>

#include <FL/panel.h>
#include <FL/gdicmn.h>
#include <FL/colour.h>
#include <FL/string.h>
#include <FL/splitter.h>
#include <FL/sizer.h>
#include <FL/frame.h>

class SplitFrame : public wxFrame
{
private:

protected:
    wxSplitterWindow* splitter;
    wxPanel* panel1;
    wxPanel* panel2;

public:

    SplitFrame( Fl_Window* parent,
                  Fl_WidgetID id = wxID_ANY,
                  const wxString& title = wxEmptyString,
                  const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxSize( 500,300 ),
                  long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL )
            : wxFrame( parent, id, title, pos, size, style )
    {
        this->SetSizeHints( wxDefaultSize, wxDefaultSize );

        wxBoxSizer* bSizer12;
        bSizer12 = new wxBoxSizer( wxHORIZONTAL );

        splitter = new wxSplitterWindow( this, wxID_ANY);
        splitter->Connect( wxEVT_IDLE, wxIdleEventHandler( SplitFrame::onIdle ), NULL, this );
        splitter->Bind( wxEVT_SPLITTER_SASH_POS_CHANGING, &SplitFrame::onSplitterMovement, this);

        panel1 = new wxPanel( splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
        panel1->SetBackgroundColour(wxColour(125,0,125));
        panel2 = new wxPanel( splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );

        // Construct a simple scenegraph.

        SoSeparator * root = new SoSeparator;

        SoCone * cone = new SoCone;
        root->addChild(cone);

        // Add the examinerviewer.

        SoFlExaminerViewer * examinerviewer = new SoFlExaminerViewer(panel2);
        examinerviewer->setSceneGraph(root);
        examinerviewer->show();

        splitter->SplitVertically( panel1, panel2, 0 );
        bSizer12->Add( splitter, 1, wxEXPAND, 5 );

        this->SetSizer( bSizer12 );
        this->Layout();

        this->Centre( wxBOTH );
    }

    ~SplitFrame() {
    }

    void onSplitterMovement(wxSplitterEvent& event) {
        std::cerr<<"move: "<<event.GetSashPosition()<<std::endl;

    }

    void onIdle( int )
    {
        splitter->SetSashPosition( 0 );
        splitter->Disconnect( wxEVT_IDLE, wxIdleEventHandler( SplitFrame::onIdle ), NULL, this );
    }

};

/***********************************************************************/

int
main(int argc, char ** argv)
{
    // Initialize SoFl.
    SoFl::init((Fl_Window *)NULL);

    // Set up scrollview window.
    SplitFrame * vp = new SplitFrame(0);

    // show window
    vp->Show();

    // start event loop.
    SoFl::mainLoop();

    // remove all resources
    SoFl::done();
    return 0;
}
