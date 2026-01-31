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

#include <Inventor/Fl/viewers/SoFlFullViewer.h>
#include "Inventor/Fl/viewers/SoFlFullViewerP.h"
#include "Inventor/Fl/widgets/SoFlThumbWheel.h"
#include <Inventor/Fl/widgets/SoFlPopupMenu.h>
#include "Inventor/Fl/SoFlInternal.h"
#include "sofldefs.h"
#include "Inventor/Fl/SoFlP.h"
#include "ButtonIndexValues.h"

// Button icons.
#include <FL/Fl_Window.H>
#include <FL/Fl_Pixmap.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Flex.H>
#include <FL/Fl_Toggle_Button.H>
#include <Inventor/Fl/common/pixmaps/pick.xpm>
#include <Inventor/Fl/common/pixmaps/view.xpm>
#include <Inventor/Fl/common/pixmaps/home.xpm>
#include <Inventor/Fl/common/pixmaps/set_home.xpm>
#include <Inventor/Fl/common/pixmaps/view_all.xpm>
#include <Inventor/Fl/common/pixmaps/seek.xpm>


#define PUBLIC(o) (o->pub)
#define PRIVATE(o) (o->pimpl)

SOFL_OBJECT_ABSTRACT_SOURCE(SoFlFullViewer);

namespace {

void onInteractClicked(Fl_Widget *, void *data) {
#if SOFL_DEBUG
    SoDebugError::postInfo("onInteractClicked", "[invoked]");
#endif
    auto *pimpl = static_cast<SoFlFullViewerP *>(data);
    if (pimpl) pimpl->interactbuttonClicked(0);
}

void onViewClicked(Fl_Widget *, void *data) {
#if SOFL_DEBUG
    SoDebugError::postInfo("onViewClicked", "[invoked]");
#endif
    auto *pimpl = static_cast<SoFlFullViewerP *>(data);
    if (pimpl) pimpl->viewbuttonClicked(0);
}

void onHomeClicked(Fl_Widget *, void *data) {
#if SOFL_DEBUG
    SoDebugError::postInfo("onViewClicked", "[invoked]");
#endif
    auto *pimpl = static_cast<SoFlFullViewerP *>(data);
    if (pimpl) pimpl->homebuttonClicked(0);
}

void onSetHomeClicked(Fl_Widget *, void *data) {
#if SOFL_DEBUG
    SoDebugError::postInfo("onSetHomeClicked", "[invoked]");
#endif
    auto *pimpl = static_cast<SoFlFullViewerP *>(data);
    if (pimpl) pimpl->sethomebuttonClicked(0);
}

void onViewAllClicked(Fl_Widget *, void *data) {
#if SOFL_DEBUG
    SoDebugError::postInfo("onViewAllClicked", "[invoked]");
#endif
    auto *pimpl = static_cast<SoFlFullViewerP *>(data);
    if (pimpl) pimpl->viewallbuttonClicked(0);
}

void onSeekClicked(Fl_Widget *, void *data) {
#if SOFL_DEBUG
    SoDebugError::postInfo("onSeekClicked", "[invoked]");
#endif
    auto *pimpl = static_cast<SoFlFullViewerP *>(data);
    if (pimpl) pimpl->seekbuttonClicked(0);
}

} // namespace

SoFlFullViewer::SoFlFullViewer(Fl_Window* parent,
                               const char* name,
                               SbBool embed,
                               BuildFlag buildFlag,
                               Type type,
                               SbBool build)
    : inherited(parent,
                name,
                embed,
                type,
                FALSE)
{
    PRIVATE(this) = new SoFlFullViewerP(this);

    PRIVATE(this)->viewerwidget = nullptr;
    PRIVATE(this)->canvas = nullptr;

    PRIVATE(this)->viewbutton = nullptr;
    PRIVATE(this)->interactbutton = nullptr;

    this->leftDecoration = nullptr;
    this->bottomDecoration = nullptr;
    this->rightDecoration = nullptr;

    this->leftWheel = nullptr;
    this->leftWheelLabel = nullptr;
    this->leftWheelStr = nullptr;
    this->leftWheelVal = 0.0f;

    this->bottomWheel = nullptr;
    this->bottomWheelLabel = nullptr;
    this->bottomWheelStr = nullptr;
    this->bottomWheelVal = 0.0f;

    this->rightWheel = nullptr;
    this->rightWheelLabel = nullptr;
    this->rightWheelStr = nullptr;
    this->rightWheelVal = 0.0f;

    this->setLeftWheelString("Motion X");
    this->setBottomWheelString("Motion Y");
    this->setRightWheelString("Dolly");

    PRIVATE(this)->mainlayout = nullptr;
    PRIVATE(this)->appbuttonlayout = nullptr;

    PRIVATE(this)->menuenabled = buildFlag & SoFlFullViewer::BUILD_POPUP;
    PRIVATE(this)->decorations = (buildFlag & SoFlFullViewer::BUILD_DECORATION) ? TRUE : FALSE;

    this->prefmenu = nullptr;
    PRIVATE(this)->menutitle = "Viewer Menu";

    PRIVATE(this)->viewerbuttons = new SbPList;
    PRIVATE(this)->appbuttonlist = new SbPList;
    PRIVATE(this)->appbuttonform = nullptr;

    if (!build) return;

    this->setClassName("SoFlFullViewer");
    Fl_Window* viewer = this->buildWidget(this->getParentWidget());
    this->setBaseWidget(viewer);
}

SoFlFullViewer::~SoFlFullViewer()
{
    delete PRIVATE(this)->viewerbuttons;
    delete PRIVATE(this)->appbuttonlist;
    delete [] this->rightWheelStr;
    delete [] this->leftWheelStr;
    delete [] this->bottomWheelStr;
    delete PRIVATE(this);
    delete this->prefmenu;
}

Fl_Window*
SoFlFullViewer::buildWidget(Fl_Window* parent)
{
    // This will build the main view widgets, along with the decorations
    // widgets and popup menu if they are enabled.
#if SOFL_DEBUG
    SoDebugError::postInfo("SoFlFullViewer::buildWidget", "[invoked]");
    parent->copy_label("MainWindow");
    SoDebugError::postInfo("SoFlFullViewer::buildWidget", "Step-1");
#ifdef SOFL_DEBUG
    SoDebugError::postInfo("SoFlFullViewer::buildWidget",
                           "%s",
                           dumpWindowData(parent).c_str());
#endif

#endif

    PRIVATE(this)->viewerwidget = parent;
    PRIVATE(this)->viewerwidget->copy_label("viewer_widget");

    this->registerWidget(PRIVATE(this)->viewerwidget);

#if SOFL_DEBUG && 0
    PRIVATE(this)->viewerwidget->color(FL_LIGHT3);
#endif

    PRIVATE(this)->canvas = inherited::buildWidget(PRIVATE(this)->viewerwidget);
    PRIVATE(this)->canvas->size(100, 100);

#if SOFL_DEBUG && 0
    PRIVATE(this)->canvas->color(FL_YELLOW);
#endif

    this->buildDecoration(PRIVATE(this)->viewerwidget);
    PRIVATE(this)->showDecorationWidgets(PRIVATE(this)->decorations);

    if (PRIVATE(this)->menuenabled)
        this->buildPopupMenu();

#if SOFL_DEBUG
    SoDebugError::postInfo("SoFlFullViewer::buildWidget", "Step-2");
#ifdef SOFL_DEBUG
    SoDebugError::postInfo("SoFlFullViewer::buildWidget",
                           "%s",
                           dumpWindowData(parent).c_str());
#endif

#endif

    PRIVATE(this)->bindEvents(PRIVATE(this)->viewerwidget);
    PRIVATE(this)->viewerwidget->resizable();
    return PRIVATE(this)->viewerwidget;
}


void
SoFlFullViewer::setDecoration(const SbBool enable)
{
#if SOFL_DEBUG
    if ((enable && this->isDecoration()) ||
        (!enable && !this->isDecoration()))
    {
        SoDebugError::postWarning("SoFlFullViewer::setDecoration",
                                  "decorations already turned %s",
                                  enable ? "on" : "off");
        return;
    }
#endif

    PRIVATE(this)->decorations = enable;
    if (PRIVATE(this)->viewerwidget)
        PRIVATE(this)->showDecorationWidgets(enable);
}

SbBool
SoFlFullViewer::isDecoration() const
{
    return (PRIVATE(this)->decorations);
}

void SoFlFullViewer::setPopupMenuEnabled(const SbBool enable)
{
#if SOFL_DEBUG
    if ((enable && this->isPopupMenuEnabled()) ||
        (!enable && !this->isPopupMenuEnabled()))
    {
        SoDebugError::postWarning("SoFlFullViewer::setPopupMenuEnabled",
                                  "popup menu already turned %s",
                                  enable ? "on" : "off");
        return;
    }
#endif
    PRIVATE(this)->menuenabled = enable;
}

SbBool
SoFlFullViewer::isPopupMenuEnabled() const
{
    return (PRIVATE(this)->menuenabled);
}

Fl_Window*
SoFlFullViewer::getAppPushButtonParent() const
{
    SOFL_STUB();
    return (0);
}

void
SoFlFullViewer::addAppPushButton(Fl_Window* newButton)
{
    SOFL_STUB();
}

void
SoFlFullViewer::insertAppPushButton(Fl_Window* newButton, int index)
{
    SOFL_STUB();
}

void
SoFlFullViewer::removeAppPushButton(Fl_Window* oldButton)
{
    SOFL_STUB();
}

int
SoFlFullViewer::findAppPushButton(Fl_Window* oldButton) const
{
    SOFL_STUB();
    return (0);
}

int
SoFlFullViewer::lengthAppPushButton() const
{
    SOFL_STUB();
    return (0);
}

Fl_Window*
SoFlFullViewer::getRenderAreaWidget() const
{
    return (PRIVATE(this)->canvas);
}

void
SoFlFullViewer::setViewing(SbBool enable)
{
    if ((enable && this->isViewing()) ||
        (!enable && !this->isViewing()))
    {
#if SOFL_DEBUG
        SoDebugError::postWarning("SoFlFullViewer::setViewing",
                                  "view mode already %s", enable ? "on" : "off");
#endif
        return;
    }

    inherited::setViewing(enable);
    /* TODO
        // Must check that buttons have been built, in case this viewer
        // component was made without decorations.
        if (PRIVATE(this)->viewerbuttons->getLength() > 0) {
            ((wxToggleButton*)(PRIVATE(this))->getViewerbutton(EXAMINE_BUTTON))->SetValue(enable);
            ((wxToggleButton*)(PRIVATE(this))->getViewerbutton(INTERACT_BUTTON))->SetValue(enable ? FALSE : TRUE);
            ((Fl_Button*)PRIVATE(this)->getViewerbutton(SEEK_BUTTON))->Enable(enable);
        }
        */
}

void
SoFlFullViewer::buildDecoration(Fl_Window* parent) {

    this->leftDecoration = this->buildLeftTrim(parent);
#if SOFL_DEBUG && 0
    this->leftDecoration->color(FL_GREEN);
#endif
    this->bottomDecoration = this->buildBottomTrim(parent);
#if SOFL_DEBUG && 0
    this->bottomDecoration->color(FL_BLUE);
#endif
    this->rightDecoration = this->buildRightTrim(parent);

#if SOFL_DEBUG && 0
    this->rightDecoration->color( FL_RED );
#endif

#if SOFL_DEBUG
    SoDebugError::postInfo("SoFlFullViewer::buildDecoration",
                           "parent: %s",
                           dumpWindowData(parent).c_str());
    SoDebugError::postInfo("SoFlFullViewer::buildDecoration",
                           "leftDecoration: %s",
                           dumpWidgetData(this->leftDecoration).c_str());
    SoDebugError::postInfo("SoFlFullViewer::buildDecoration",
                           "rightDecoration: %s",
                           dumpWidgetData(this->rightDecoration).c_str());
    SoDebugError::postInfo("SoFlFullViewer::buildDecoration",
                           "bottomDecoration: %s",
                           dumpWidgetData(this->bottomDecoration).c_str());
#endif

    this->buildViewerButtons(parent);
    PRIVATE(this)->initThumbWheelEventMap();
}

/*
    // Centratura logica
    int x = parent->x() + (parent->w() - t->w()) / 2;
    int y = parent->y() + (parent->h() - t->h()) / 2;
*/

Fl_Window*
SoFlFullViewer::buildLeftTrim(Fl_Window* parent)
{
    //auto
    auto t = new SoFlThumbWheel(SoFlThumbWheel::Vertical, SbVec2s(0,0));
    //int y = (parent->h() - t->h()) / 2;
    //t->position(0, y);
    t->copy_label("left thumb wheel");
    t->setRangeBoundaryHandling(SoFlThumbWheel::ACCUMULATE);
    this->leftWheelVal = t->value();
    this->leftWheel = t;
    return t;
}

Fl_Window*
SoFlFullViewer::buildBottomTrim(Fl_Window* parent)
{
    auto t = new SoFlThumbWheel(SoFlThumbWheel::Horizontal, SbVec2s(0,0));
    /*
     *int x = (parent->w() - t->w()) / 2;
    int y = parent->h() - t->h();
    t->position(x, y);
    */
    t->copy_label("bottom thumb wheel");
    t->setRangeBoundaryHandling(SoFlThumbWheel::ACCUMULATE);
    this->bottomWheelVal = t->value();
    this->bottomWheel = t;
    return t;
}

Fl_Window*
SoFlFullViewer::buildRightTrim(Fl_Window* parent)
{
    auto t = new SoFlThumbWheel(SoFlThumbWheel::Vertical, SbVec2s(0,0));
    /*
    int x = parent->w() - t->w();
    int y = (parent->h() - t->h()) / 2;
    t->position(x, y);
    */
    t->copy_label("right thumb wheel");
    t->setRangeBoundaryHandling(SoFlThumbWheel::ACCUMULATE);
    this->rightWheelVal = t->value();
    this->rightWheel = t;
    return t;
}

Fl_Window*
SoFlFullViewer::buildAppButtons(Fl_Window* parent)
{
    SOFL_STUB();
    return (0);
}

Fl_Window*
SoFlFullViewer::buildViewerButtons(Fl_Window* parent)
{
    if (!parent) return nullptr;

    parent->begin();
    this->createViewerButtons(parent, PRIVATE(this)->viewerbuttons);
    parent->end();

    PRIVATE(this)->layoutViewerButtons(this, SbVec2s(parent->w(), parent->h()));
    return parent;
}

void
SoFlFullViewer::createViewerButtons(Fl_Window* parent,
                                    SbPList* button_list)
{
    if (!parent || !button_list) return;

    static Fl_Pixmap pick_pixmap(const_cast<char**>(pick_xpm));
    static Fl_Pixmap view_pixmap(const_cast<char**>(view_xpm));
    static Fl_Pixmap home_pixmap(const_cast<char**>(home_xpm));
    static Fl_Pixmap set_home_pixmap(const_cast<char**>(set_home_xpm));
    static Fl_Pixmap view_all_pixmap(const_cast<char**>(view_all_xpm));
    static Fl_Pixmap seek_pixmap(const_cast<char**>(seek_xpm));

    auto *interact = new Fl_Toggle_Button(0, 0, XPM_BUTTON_SIZE, XPM_BUTTON_SIZE);
    interact->image(&pick_pixmap);
    interact->value(this->isViewing() ? 0 : 1);
    interact->label(nullptr);
    interact->callback(&onInteractClicked, PRIVATE(this));
    PRIVATE(this)->interactbutton = interact;
    button_list->append(interact);

    auto *view = new Fl_Toggle_Button(0, 0, XPM_BUTTON_SIZE, XPM_BUTTON_SIZE);
    view->image(&view_pixmap);
    view->value(this->isViewing() ? 1 : 0);
    view->label(nullptr);
    view->callback(&onViewClicked, PRIVATE(this));
    PRIVATE(this)->viewbutton = view;
    button_list->append(view);

    auto *home = new Fl_Button(0, 0, XPM_BUTTON_SIZE, XPM_BUTTON_SIZE);
    home->image(&home_pixmap);
    home->label(nullptr);
    home->callback(&onHomeClicked, PRIVATE(this));
    button_list->append(home);

    auto *set_home = new Fl_Button(0, 0, XPM_BUTTON_SIZE, XPM_BUTTON_SIZE);
    set_home->image(&set_home_pixmap);
    set_home->label(nullptr);
    set_home->callback(&onSetHomeClicked, PRIVATE(this));
    button_list->append(set_home);

    auto *view_all = new Fl_Button(0, 0, XPM_BUTTON_SIZE, XPM_BUTTON_SIZE);
    view_all->image(&view_all_pixmap);
    view_all->label(nullptr);
    view_all->callback(&onViewAllClicked, PRIVATE(this));
    button_list->append(view_all);

    auto *seek = new Fl_Button(0, 0, XPM_BUTTON_SIZE, XPM_BUTTON_SIZE);
    seek->image(&seek_pixmap);
    seek->label(nullptr);
    seek->callback(&onSeekClicked, PRIVATE(this));
    button_list->append(seek);
}

void
SoFlFullViewer::buildPopupMenu()
{
    this->prefmenu = PRIVATE(this)->setupStandardPopupMenu();
}

void
SoFlFullViewer::openPopupMenu(const SbVec2s position)
{
    if (!this->isPopupMenuEnabled()) return;
    if (this->prefmenu == nullptr)
        this->buildPopupMenu();
    int x = 2 + position[0];
    int y = 2 + this->getGLSize()[1] - position[1] - 1;

    PRIVATE(this)->prepareMenu(this->prefmenu);
    this->prefmenu->popUp(this->getGLWidget(), x, y);
}

void
initString(char* & destination,
           const char* data)
{
    delete [] destination;
    destination = nullptr;
    if (data)
    {
        const auto length = strlen(data) + 1;
        destination = strncpy(new char[length], data, length);
    }
}

void
SoFlFullViewer::setLeftWheelString(const char* const name)
{
    initString(this->leftWheelStr, name);
    if (this->leftWheelLabel)
        this->leftWheelLabel->copy_label(name ? name : "");
}

void
SoFlFullViewer::setBottomWheelString(const char* const name)
{
    initString(this->bottomWheelStr, name);
    if (this->bottomWheelLabel)
        this->bottomWheelLabel->copy_label(name ? name : "");
}

void
SoFlFullViewer::setRightWheelString(const char* const name)
{
    initString(this->rightWheelStr, name);
    if (this->rightWheelLabel)
    {
        this->rightWheelLabel->copy_label(name ? name : "");
    }
}

void
SoFlFullViewer::sizeChanged(const SbVec2s& size)
{
    if (size[0] <= 0 || size[1] <= 0) return;

#if SOFL_DEBUG
    SoDebugError::postInfo("SoFlFullViewer::sizeChanged", "(%d, %d)",
                           size[0], size[1]);
#endif

    if (PRIVATE(this)->viewerwidget) {
        int left_trim_w = 0;
        int right_trim_w = 0;
        int bottom_trim_h = 0;

        if (PRIVATE(this)->decorations) {
            // leftDecoration -> basso a sinistra
            if (this->leftWheel) {
                left_trim_w = leftWheel->w();
                int y = size[1] - this->leftWheel->h();
                this->leftWheel->position(0, y);
            }
            // rightDecoration -> basso a destra (sotto i bottoni)
            if (this->rightWheel) {
                right_trim_w = rightWheel->w();
                int x = size[0] - this->rightWheel->w();
                int y = size[1] - this->rightWheel->h();
                this->rightWheel->position(x, y);
            }
            // bottomDecoration -> al centro, in basso
            if (this->bottomWheel) {
                bottom_trim_h = bottomWheel->h();
                int x = left_trim_w + (size[0] - left_trim_w - right_trim_w - this->bottomWheel->w()) / 2;
                int y = size[1] - this->bottomWheel->h();
                this->bottomWheel->position(x, y);
            }
        }

        // layoutViewerButtons posiziona i bottoni in alto a destra
        PRIVATE(this)->layoutViewerButtons(this, size);

        // canvas -> centro, sopra bottomDecoration
        if (PRIVATE(this)->canvas) {
            // Occupa tutto lo spazio tranne i trim laterali (se vogliamo che copra i trim laterali in altezza)
            // L'utente dice: "canvas e bottomDecoration devono essere al centro con canvas sopra e bottomDecoration sotto"
            // Quindi il canvas deve finire dove inizia bottomDecoration.
            PRIVATE(this)->canvas->resize(left_trim_w, 0, size[0] - left_trim_w - right_trim_w, size[1] - bottom_trim_h);
        }
    }

    inherited::sizeChanged(size);
}

const char*
SoFlFullViewer::getRightWheelString() const
{
    return (this->rightWheelStr);
}

#undef PUBLIC
#undef PRIVATE
