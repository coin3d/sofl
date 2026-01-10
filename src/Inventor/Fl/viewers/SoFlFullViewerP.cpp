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

#include "Inventor/Fl/viewers/SoFlFullViewerP.h"
#include "Inventor/Fl/viewers/SoFlFullViewer.h"
#include "Inventor/Fl/widgets/WheelEvents.h"
#include "Inventor/Fl/widgets/SoFlThumbWheel.h"
#include "Inventor/Fl/SoFlInternal.h"
#include "ButtonIndexValues.h"
#include "sofldefs.h"
#include <FL/Fl_Window.H>


#define PUBLIC(o) (o->pub)
#define PRIVATE(o) (o->pimpl)

SoFlFullViewerP::SoFlFullViewerP(SoFlFullViewer *pViewer)
        : SoGuiFullViewerP(pViewer) {
    assert(pViewer);
}

SoFlFullViewerP::~SoFlFullViewerP() {
    objectMap.clear();
}

void
SoFlFullViewerP::setThumbWheelValue(Fl_Widget* wheel, float val) {
    dynamic_cast<SoFlThumbWheel*>(wheel)->setValue(val);
}

void
SoFlFullViewerP::showDecorationWidgets(SbBool onOff) {
#if SOFL_DEBUG
    SoDebugError::postInfo("SoFlFullViewerP::showDecorationWidgets", "[invoked] %s", onOff ? "ON" : "OFF");
#endif
    if (onOff) {
        if (PUBLIC(this)->leftDecoration) PUBLIC(this)->leftDecoration->show();
        if (PUBLIC(this)->bottomDecoration) PUBLIC(this)->bottomDecoration->show();
        if (PUBLIC(this)->rightDecoration) PUBLIC(this)->rightDecoration->show();
    } else {
        if (PUBLIC(this)->leftDecoration) PUBLIC(this)->leftDecoration->hide();
        if (PUBLIC(this)->bottomDecoration) PUBLIC(this)->bottomDecoration->hide();
        if (PUBLIC(this)->rightDecoration) PUBLIC(this)->rightDecoration->hide();
    }
    if (this->viewerwidget) {
        SbVec2s size(this->viewerwidget->w(), this->viewerwidget->h());
        PUBLIC(this)->sizeChanged(size);
    }
}

void
SoFlFullViewerP::wheelPressed(int event) {
#if SOFL_DEBUG
    SoDebugError::postInfo("SoFlFullViewerP::wheelPressed", "event arrived!");
#endif
    MapEvent::iterator it = objectMap.find( static_cast<Fl_Widget*>(nullptr));//event.GetEventObject()));
    if( it != objectMap.end() ) {
        VoidFuncNoPar function = it->second.onPress;
        (PUBLIC(this)->*function)();
    }

#if SOFL_DEBUG
    else {
        SoDebugError::postWarning("SoFlFullViewerP::wheelPressed", "not valid event found!");
    }
#endif


}

void
SoFlFullViewerP::wheelReleased(int event) {
#if SOFL_DEBUG
    SoDebugError::postInfo("SoFlFullViewerP::wheelReleased", "event arrived!");
#endif
    MapEvent::iterator it = objectMap.find( static_cast<Fl_Widget*>(nullptr));//event.GetEventObject()));
    if( it != objectMap.end() ) {
        VoidFuncNoPar function = it->second.onRelease;
        (PUBLIC(this)->*function)();
    }

#if SOFL_DEBUG
    else {
        SoDebugError::postWarning("SoFlFullViewerP::wheelReleased", "not valid event found!");
    }
#endif

}

void
SoFlFullViewerP::wheelMoved(int event) {
#if SOFL_DEBUG
    SoDebugError::postInfo("SoFlFullViewerP::wheelMoved", "event arrived!");
#endif

    Fl_Widget* emitting_thumb_wheel = nullptr; //static_cast<Fl_Widget*>(event.GetEventObject());
    MapEvent::iterator it = objectMap.find( emitting_thumb_wheel);
    if( it != objectMap.end() ) {
        VoidFuncOnePar function = it->second.onMove;
        float *v = nullptr;//static_cast<float*>(event.GetClientData());
        if(v)
            (PUBLIC(this)->*function)(*v);
    }

#if SOFL_DEBUG
    else {
        SoDebugError::postWarning("SoFlFullViewerP::wheelMoved", "not valid event found!");
    }
#endif


}

#define ADD_DATA_IN_MAP(objectName) \
assert(PUBLIC(this)->objectName && "Thumbwheel Can no be null"); \
objectMap[PUBLIC(this)->objectName] = WheelFunctions(&SoFlFullViewer::objectName ## Start,\
                                                     &SoFlFullViewer::objectName ## Finish, \
                                                     &SoFlFullViewer::objectName ## Motion)
void
SoFlFullViewerP::initThumbWheelEventMap() {
    ADD_DATA_IN_MAP(rightWheel);
    ADD_DATA_IN_MAP(leftWheel);
    ADD_DATA_IN_MAP(bottomWheel);
}

void
SoFlFullViewerP::homebuttonClicked(int ) {
    PUBLIC(this)->resetToHomePosition();
}

void
SoFlFullViewerP::sethomebuttonClicked(int) {
    PUBLIC(this)->saveHomePosition();
}

void
SoFlFullViewerP::viewallbuttonClicked(int) {
    PUBLIC(this)->viewAll();
}

void
SoFlFullViewerP::seekbuttonClicked(int) {
    PUBLIC(this)->setSeekMode(PUBLIC(this)->isSeekMode() ? FALSE : TRUE);
}

void
SoFlFullViewerP::interactbuttonClicked(int) {

    if (this->interactbutton)
        this->interactbutton->value(TRUE);

    if (this->viewbutton)
        this->viewbutton->value(FALSE);

    if (PUBLIC(this)->isViewing())
        PUBLIC(this)->setViewing(FALSE);
}

void
SoFlFullViewerP::seekbuttonClicked() {
    PUBLIC(this)->setSeekMode(PUBLIC(this)->isSeekMode() ? FALSE : TRUE);
}

void
SoFlFullViewerP::increaseInteractiveCount() {
    PUBLIC(this)->interactiveCountInc();
}

void
SoFlFullViewerP::decreaseInteractiveCount() {
    PUBLIC(this)->interactiveCountDec();
}

void
SoFlFullViewerP::viewbuttonClicked(int) {
    if (this->interactbutton)
        this->interactbutton->value(FALSE);
    if (this->viewbutton)
        this->viewbutton->value(TRUE);

    if (!PUBLIC(this)->isViewing())
        PUBLIC(this)->setViewing(TRUE);

}

void SoFlFullViewerP::bindEvents(Fl_Window *w) {
    assert(w && "window can not be null!");
    /*
    w->Bind(SO_WX_MOUSE_WHEEL_PRESSED, &SoFlFullViewerP::wheelPressed, this, FULL_VIEWER_P);
    w->Bind(SO_WX_MOUSE_WHEEL_RELEASED, &SoFlFullViewerP::wheelReleased, this, FULL_VIEWER_P);
    w->Bind(SO_WX_MOUSE_WHEEL_MOVED, &SoFlFullViewerP::wheelMoved, this, FULL_VIEWER_P);

    w->Bind(wxEVT_TOGGLEBUTTON, &SoFlFullViewerP::interactbuttonClicked, this, INTERACT_BUTTON);
    w->Bind(wxEVT_TOGGLEBUTTON, &SoFlFullViewerP::viewbuttonClicked, this, EXAMINE_BUTTON);

    w->Bind(wxEVT_BUTTON, &SoFlFullViewerP::homebuttonClicked, this, HOME_BUTTON);
    w->Bind(wxEVT_BUTTON, &SoFlFullViewerP::sethomebuttonClicked, this, SET_HOME_BUTTON);
    w->Bind(wxEVT_BUTTON, &SoFlFullViewerP::seekbuttonClicked, this, SEEK_BUTTON);
    w->Bind(wxEVT_BUTTON, &SoFlFullViewerP::viewallbuttonClicked, this, VIEW_ALL_BUTTON);
    */
}

#undef ADD_DATA_IN_MAP
#undef PUBLIC
#undef PRIVATE
