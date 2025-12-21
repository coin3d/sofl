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

#include "Inventor/Fl/SoFl.h"
#include "Inventor/Fl/SoFlP.h"
#include <fl/event.h>
#include "Inventor/Fl/SoFlInternal.h"
#include "sowxdefs.h"

wxWindow*
SoFl::init(int & argc,
           char ** argv,
           const char * appname,
           const char * classname) {

    // retrieve the instance
    SoFlP::instance();

    if (SOWX_DEBUG && SoFlP::instance()->isInitialized()) {
        SoDebugError::postWarning("SoFl::init",
                                  "This method should be called only once.");
        return SoFlP::instance()->getMainFrame();
    }

    // Call all the code for initializing Coin data
    SoFlP::commonInit();

    // if wxApp is not already created
    if (wxApp::GetInstance() == NULL) {
        // Set up the QApplication instance which we have derived into a
        // subclass to catch spaceball events.
        SoFlP::instance()->buildWxApp();
        wxApp::SetInstance(SoFlP::instance()->main_app);
        wxEntryStart( argc, argv );
        wxTheApp->CallOnInit();
    }
    else {
        // The user already set one up for us.
        // so nothing to do
        SoFlP::instance()->setWxApp(wxApp::GetInstance());
    }

    SoFlP::instance()->setMainFrame( new wxFrame(0,
                                                   wxID_ANY,
                                                   appname));

    assert(SoFlP::instance());
    assert(wxTheApp);
    wxTheApp->Bind(wxEVT_IDLE, &SoFlP::onIdle,  SoFlP::instance());
    SoFlP::instance()->getMainFrame()->Bind(wxEVT_CLOSE_WINDOW, &SoFlP::onClose,  SoFlP::instance());

    SoDB::getSensorManager()->setChangedCallback(SoGuiP::sensorQueueChanged,
                                                 NULL);

    SoFlP::instance()->setInitialize(true);
#ifdef SOWX_DEBUG
    SoDebugError::postInfo("SoFl::init",
                           "%s",
                           dumpWindowData(SoFlP::instance()->getMainFrame()).c_str());
#endif
    return (SoFlP::instance()->getMainFrame());
}

void
SoFl::init(wxWindow* toplevelwidget) {
#ifdef COIN_IV_EXTENSIONS
#define COIN_IV_EXTENSION(ext) ext::initClass();
    COIN_IV_EXTENSIONS
#undef COIN_IV_EXTENSION
#endif

    if (SOWX_DEBUG && SoFlP::instance()->isInitialized()) {
        SoDebugError::postWarning("SoFl::init",
                                  "This method should be called only once.");
        return;
    }

    // Call all the code for initializing Coin data
    SoFlP::commonInit();

    // if wxApp is not already created
    if (wxApp::GetInstance() == NULL) {
        SoFlP::instance()->buildWxApp();
        wxApp::SetInstance(SoFlP::instance()->main_app);
        static const char * dummyargv[1];
        dummyargv[0] = "SoFl";
        int argc = 1;
        wxEntryStart( argc, (char **)dummyargv );
        wxTheApp->CallOnInit();
    }
    else {
        // The user already set one up for us.
        // so nothing to do
        SoFlP::instance()->setWxApp(wxApp::GetInstance());
    }

    wxTheApp->Bind(wxEVT_IDLE, &SoFlP::onIdle,  SoFlP::instance());
    if(toplevelwidget) {
        SoFlP::instance()->setMainFrame(toplevelwidget);
        SoFlP::instance()->getMainFrame()->Bind(wxEVT_CLOSE_WINDOW, &SoFlP::onClose, SoFlP::instance());
    }
    SoDB::getSensorManager()->setChangedCallback(SoGuiP::sensorQueueChanged,
                                                 NULL);

    SoFlP::instance()->setInitialize(true);
#ifdef SOWX_DEBUG
    SoDebugError::postInfo("SoFl::init",
                           "%s",
                           dumpWindowData(SoFlP::instance()->getMainFrame()).c_str());
#endif
}

/**
 * mainLoop is required only if an external app is not available
 * (embedded in already existing window).
 */
void
SoFl::mainLoop(void) {
    wxTheApp->OnRun();
}


void
SoFl::exitMainLoop(void)  {
    wxExit();
}

void
SoFl::done() {
    SoFlP::instance()->finish();
#if 0 // FIXME: These methods exist in TGS Inventor. We should add
    // them, and then call them from here. 20060210 kyrah
  SoInteraction::finish();
  SoNodeKit::finish();
#endif
    SoDB::finish();
}

void
SoFl::show(wxWindow* const widget) {
    widget->Show();
}

void
SoFl::hide(wxWindow* const widget) {
    widget->Hide();
}

void
SoFl::createSimpleErrorDialog(wxWindow* widget,
                              const char * title,
                              const char * string1,
                              const char * string2 ) {
    SOWX_STUB();
}

wxWindow*
getTopLevelWidget(void) {
    return (wxTheApp->GetTopWindow());
}

wxWindow*
SoFl::getShellWidget(const wxWindow* w) {
#if 0
    return (wxGetTopLevelParent((wxWindowBase *) w));
#else
    wxWindow* p = const_cast<wxWindow*>(w);
    while (p !=  NULL) {
        wxFrame* top_frame = dynamic_cast<wxFrame*>(p);
        if ( top_frame ) {
            return p;
        }
        p = p->GetParent();
    }
#if SOWX_DEBUG && 0 // debug
    SoDebugError::postInfo("SoFl::getShellWidget",
                         "couldn't find shell for widget at %p", widget);
#endif // debug
    return (NULL);
#endif
}

void
SoFl::setWidgetSize(wxWindow* const widget, const SbVec2s size) {
    assert(widget != 0 && "widget can not be null");
    if ( widget ) {
        widget->SetSize(size[0], size[1]);
    }
#if SOWX_DEBUG
    else  {
        SoDebugError::postWarning("SoFl::setWidgetSize",
                                  "null widget on setting: <%d, %d>.",
                                  size[0], size[1]);
    }
#endif // SOWX_DEBUG
}


SbVec2s
SoFl::getWidgetSize(const wxWindow* widget) {
    assert(widget != 0 && "widget can not be null");
    SbVec2s size(-1,-1);
    if ( widget ) {
        wxSize wx_size = widget->GetSize();
        size[0] = wx_size.GetWidth();
        size[1] = wx_size.GetHeight();
    }
#if SOWX_DEBUG
    else  {
        SoDebugError::postWarning("SoFl::getWidgetSize",
                                  "null widget");
    }
#endif // SOWX_DEBUG
}
