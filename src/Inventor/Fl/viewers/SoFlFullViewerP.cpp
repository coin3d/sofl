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
#include "Inventor/Fl/widgets/SoFlThumbWheel.h"
#include "ButtonIndexValues.h"
#include "sofldefs.h"
#include <FL/Fl_Window.H>


#define PUBLIC(o) (o->pub)
#define PRIVATE(o) (o->pimpl)

SoFlFullViewerP::SoFlFullViewerP(SoFlFullViewer *pViewer)
        : SoGuiFullViewerP(pViewer)
{
    assert(pViewer);
}

SoFlFullViewerP::~SoFlFullViewerP()
{
    while (!objectMap.empty())
    {
        delete objectMap.back();
        objectMap.pop_back();
    }
}

void
SoFlFullViewerP::setThumbWheelValue(Fl_Window* wheel, float val)
{
    dynamic_cast<SoFlThumbWheel*>(wheel)->setValue(val);
}

void
SoFlFullViewerP::showDecorationWidgets(SbBool onOff)
{
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
SoFlFullViewerP::wheelPressed(int event)
{
#if SOFL_DEBUG
    SoDebugError::postInfo("SoFlFullViewerP::wheelPressed", "event arrived!");
#endif

}

void
SoFlFullViewerP::wheelReleased(int event)
{
#if SOFL_DEBUG
    SoDebugError::postInfo("SoFlFullViewerP::wheelReleased", "event arrived!");
#endif

}

void
SoFlFullViewerP::wheelMoved(int event)
{
#if SOFL_DEBUG
    SoDebugError::postInfo("SoFlFullViewerP::wheelMoved", "event arrived!");
#endif

}


#define ADD_DATA_IN_MAP(objectName) do {\
assert(PUBLIC(this)->objectName && "Thumbwheel Can no be null"); \
auto wheelFunctions = new WheelFunctions(PUBLIC(this), \
                                        &SoFlFullViewer::objectName ## Start,\
                                        &SoFlFullViewer::objectName ## Finish,\
                                        &SoFlFullViewer::objectName ## Motion);\
objectMap.push_back(wheelFunctions);\
PUBLIC(this)->objectName->user_data(wheelFunctions); } while (0)


void
SoFlFullViewerP::initThumbWheelEvent() {
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

void SoFlFullViewerP::layoutViewerButtons(SoFlFullViewer *viewer, const SbVec2s &size) {
    if (!viewer) return;
    if (!PRIVATE(viewer)->viewerbuttons) return;

    const int count = PRIVATE(viewer)->viewerbuttons->getLength();
    if (count <= 0) return;

    const bool show = PRIVATE(viewer)->decorations ? true : false;
    const int x = size[0] - XPM_BUTTON_SIZE - XPM_BUTTON_PADDING;
    int y = XPM_BUTTON_PADDING;

    for (int i = 0; i < count; ++i) {
        auto *w = static_cast<Fl_Widget*>((*PRIVATE(viewer)->viewerbuttons)[i]);
        if (!w) continue;
        w->size(XPM_BUTTON_SIZE, XPM_BUTTON_SIZE);
        w->position(x, y);
        if (show) w->show();
        else w->hide();
        y += XPM_BUTTON_SIZE + XPM_BUTTON_PADDING;
    }
}

#undef PUBLIC
#undef PRIVATE
