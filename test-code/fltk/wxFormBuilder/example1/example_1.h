///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <fl/artprov.h>
#include <fl/xrc/xmlres.h>
#include <fl/statusbr.h>
#include <fl/gdicmn.h>
#include <fl/font.h>
#include <fl/colour.h>
#include <fl/settings.h>
#include <fl/string.h>
#include <fl/bitmap.h>
#include <fl/image.h>
#include <fl/icon.h>
#include <fl/menu.h>
#include <fl/panel.h>
#include <fl/sizer.h>
#include <fl/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class AFrame
///////////////////////////////////////////////////////////////////////////////
class AFrame : public wxFrame
{
	private:

	protected:
		wxStatusBar* status_bar;
		wxMenuBar* menu_bar;
		wxMenu* file;
		wxPanel* panel;

		// Virtual event handlers, override them in your derived class
		virtual void on_about( wxCommandEvent& event ) { event.Skip(); }
		virtual void on_quit( wxCommandEvent& event ) { event.Skip(); }
		virtual void on_size( wxSizeEvent& event ) { event.Skip(); }


	public:

		AFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~AFrame();

};

