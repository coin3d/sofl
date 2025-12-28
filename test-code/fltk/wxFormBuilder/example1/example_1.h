///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <FL/artprov.h>
#include <FL/xrc/xmlres.h>
#include <FL/statusbr.h>
#include <FL/gdicmn.h>
#include <FL/font.h>
#include <FL/colour.h>
#include <FL/settings.h>
#include <FL/string.h>
#include <FL/bitmap.h>
#include <FL/image.h>
#include <FL/icon.h>
#include <FL/menu.h>
#include <FL/panel.h>
#include <FL/sizer.h>
#include <FL/frame.h>

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
		virtual void on_about( int event ) { event.Skip(); }
		virtual void on_quit( int event ) { event.Skip(); }
		virtual void on_size( int event ) { event.Skip(); }


	public:

		AFrame( Fl_Widget* parent, Fl_WidgetID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~AFrame();

};

