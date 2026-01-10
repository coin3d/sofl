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

#include "Inventor/Fl/SoFlComponent.h"
#include "Inventor/Fl/SoFlComponentP.h"
#include "Inventor/Fl/SoAny.h"
#include "Inventor/Fl/SoFlGLWidget.h"
#include "Inventor/Fl/SoFlRenderArea.h"
#include "Inventor/Fl/SoFlCursor.h"

#include "Inventor/Fl/viewers/SoFlViewer.h"
#include "Inventor/Fl/viewers/SoFlFullViewer.h"
#include "Inventor/Fl/viewers/SoFlExaminerViewer.h"
#include "Inventor/Fl/viewers/SoFlPlaneViewer.h"
#include "Inventor/Fl/viewers/SoFlConstrainedViewer.h"
#include "Inventor/Fl/viewers/SoFlFlyViewer.h"

#include "Inventor/Fl/SoFlInternal.h"

#include "sofldefs.h"
#include "SoFlP.h"

#define SOFLCOMP_RESIZE_DEBUG 1

#define PRIVATE(obj) ((obj)->pimpl)
#define PUBLIC(obj) ((obj)->pub)

SOFL_OBJECT_ABSTRACT_SOURCE(SoFlComponent);

SoFlComponent::~SoFlComponent(){

}

SoFlComponent::SoFlComponent(Fl_Window* const parent,
                             const char * const name,
                             const SbBool embed) {
    PRIVATE(this) = new SoFlComponentP(this);

    PRIVATE(this)->realized = false;
    PRIVATE(this)->shelled = false;
    PRIVATE(this)->widget = nullptr;
    PRIVATE(this)->parent = parent;
    PRIVATE(this)->closeCB = nullptr;
    PRIVATE(this)->closeCBdata = nullptr;
    PRIVATE(this)->visibilitychangeCBs = nullptr;
    PRIVATE(this)->fullscreen = false;

    this->setClassName("SoFlComponent");

    PRIVATE(this)->storesize.setValue(-1, -1);

    SoAny::si()->addInternalFatalErrorHandler(SoFlComponentP::fatalerrorHandler,
                                              PRIVATE(this));

    PRIVATE(this)->widgetname = (name ? name :
                                 this->getDefaultWidgetName());

    if (!parent || !embed) {
        // TODO: PRIVATE(this)->parent = new wxFrame(nullptr, wxID_ANY, name);
        PRIVATE(this)->embedded = false;
        PRIVATE(this)->shelled = true;
    }
    else {
        PRIVATE(this)->parent = parent;
        PRIVATE(this)->embedded = true;
    }

    // TODO: PRIVATE(this)->parent->installEventFilter(PRIVATE(this));
}

void  SoFlComponent::initClasses() {
    SoFlComponent::initClass();
    SoFlGLWidget::initClass();
    SoFlRenderArea::initClass();
    SoFlViewer::initClass();
    SoFlExaminerViewer::initClass();
    SoFlPlaneViewer::initClass();
    SoFlConstrainedViewer::initClass();
    SoFlFullViewer::initClass();
    SoFlFlyViewer::initClass();
}

void
SoFlComponent::afterRealizeHook() {
    SOFL_STUB();
}

void
SoFlComponent::setClassName(const char * const name) {
    PRIVATE(this)->classname = name;
}

void
SoFlComponent::setBaseWidget(Fl_Window* w) {

#ifdef SOFL_DEBUG
    SoDebugError::postInfo("SoFl::setBaseWidget",
                           "%s",
                           dumpWindowData(w).c_str());
#endif

    std::string iconText = this->getDefaultIconTitle();
    std::string widgetName = PRIVATE(this)->widgetname;

    assert(w);

    if (PRIVATE(this)->widget) {
        /* TODO: iconText = (PRIVATE(this)->widget-windowIconText().isEmpty() ?
		iconText :
		PRIVATE(this)->widget->windowIconText());
        widgetName = (PRIVATE(this)->widget->GetName().IsEmpty() ?
                      widgetName :
                      PRIVATE(this)->widget->GetName());
        */

        this->unregisterWidget(PRIVATE(this)->widget);
    }

    PRIVATE(this)->widget = w;

    this->registerWidget(PRIVATE(this)->widget);

#if SOFL_DEBUG && 0
    SoDebugError::postInfo("SoFlComponent::setBaseWidget",
                           "widget: %p, parent: %p", w, PRIVATE(this)->parent);
#endif

    if (!PRIVATE(this)->parent /*|| PRIVATE(this)->parent->IsTopLevel()*/) {

        const char* label = PRIVATE(this)->widget->label();
        if (label == nullptr || label[0] == '\0') {
            this->setTitle(this->getDefaultTitle());
        }

        SoFl::getShellWidget(this->getWidget())->copy_label(iconText.c_str());
    }
    PRIVATE(this)->widget->copy_label(widgetName.c_str());
}

void
SoFlComponent::show() {
    if(SOFL_DEBUG && !PRIVATE(this)->widget) { // debug
        SoDebugError::postWarning("SoFlComponent::show",
                                  "Called while no Fl_Window has been set.");
        return;
    }

    if (SOFLCOMP_RESIZE_DEBUG) {  // debug
        SoDebugError::postInfo("SoFlComponent::show-1",
                               "resizing %p: (%d, %d)",
                               PRIVATE(this)->widget,
                               PRIVATE(this)->storesize[0],
                               PRIVATE(this)->storesize[1]);
    }

    if (PRIVATE(this)->storesize[0] > 0 && PRIVATE(this)->storesize[1] > 0) {
        // only parent has power for setting the correct size
        PRIVATE(this)->parent->size(PRIVATE(this)->storesize[0],
                                             PRIVATE(this)->storesize[1]);
    }

    if (SOFLCOMP_RESIZE_DEBUG) {  // debug
        SoDebugError::postInfo("SoFlComponent::show-2",
                               "resized %p: (%d, %d)",
                               PRIVATE(this)->widget,
                               PRIVATE(this)->widget->w(),
                               PRIVATE(this)->widget->h());
    }

    if(! PRIVATE(this)->embedded && PRIVATE(this)->shelled) {
#if SOFL_DEBUG
        SoDebugError::postInfo("SoFlComponent::show",
                               "perform show if is not embedded and is shelled");
        PRIVATE(this)->parent->show();
#endif
    }
    PRIVATE(this)->widget->show();

    this->sizeChanged(PRIVATE(this)->storesize);
}

void
SoFlComponent::hide() {
    SOFL_STUB();
    PRIVATE(this)->widget->hide();
}

void
SoFlComponent::setComponentCursor(const SoFlCursor & cursor) {
    SOFL_STUB();
    SoFlComponent::setWidgetCursor(this->getWidget(), cursor);
}

void
SoFlComponent::setWidgetCursor(Fl_Window* w, const SoFlCursor & cursor) {

    if(!w) {
        return;
    }
    // FIXME: as this function is called all the time when the cursor is
    // grabbed by the window under X11, we should really compare with
    // the previous cursor before doing anything, to avoid spending
    // unnecessary clockcycles during animation. 20011203 mortene.
/*
    if (cursor.getShape() == SoFlCursor::CUSTOM_BITMAP) {
        const SoFlCursor::CustomCursor * cc = &cursor.getCustomCursor();
        w->SetCursor(*SoFlComponentP::getNativeCursor(cc));
    }
    else {
        switch (cursor.getShape()) {
            case SoFlCursor::DEFAULT:
                w->SetCursor(*wxSTANDARD_CURSOR);
                break;

            case SoFlCursor::BUSY:
                w->SetCursor(*wxHOURGLASS_CURSOR);
                break;

            case SoFlCursor::CROSSHAIR:
                w->SetCursor(*wxCROSS_CURSOR);
                break;

            case SoFlCursor::UPARROW:
                w->SetCursor(*wxCROSS_CURSOR);
                break;

            default:
                assert(false && "unsupported cursor shape type");
                break;
        }
    }
    */
}

SbBool
SoFlComponent::isFullScreen() const {
    return (PRIVATE(this)->fullscreen);
}

SbBool
SoFlComponent::setFullScreen(const SbBool onoff) {
    Fl_Window * w = this->getShellWidget();
    if (w == nullptr) w = this->getParentWidget();
    if (w == nullptr) w = this->getWidget();
    if (!w) { return false; }
    /*
    wxFrame* frame =  dynamic_cast<wxFrame*>(w);
    if(frame) {
        frame->ShowFullScreen(onoff);
    }
    else {
        return (false);
    }
    */

    PRIVATE(this)->fullscreen = onoff;
    return (PRIVATE(this)->fullscreen);
}

SbBool
SoFlComponent::isVisible() {
    bool ret = false;
    if( PRIVATE(this)->widget ) {
        ret = PRIVATE(this)->widget->visible();
    }
    return (ret);
}

SbBool
SoFlComponent::isTopLevelShell() const {
#if SOFL_DEBUG
    if (! PRIVATE(this)->widget) {
        SoDebugError::postWarning("SoFlComponent::isTopLevelShell",
                                  "Called while no Fl_Window has been set.");
        return false;
    }
#endif // SOFL_DEBUG
    return PRIVATE(this)->embedded ? false : true;
}

Fl_Window*
SoFlComponent::getWidget() const {
    return this->getBaseWidget();
}

Fl_Window*
SoFlComponent::getBaseWidget() const {
    return PRIVATE(this)->widget;
}

Fl_Window*
SoFlComponent::getParentWidget() const {
    return PRIVATE(this)->parent;
}

void
SoFlComponent::setSize(const SbVec2s size) {

#ifdef SOFL_DEBUG
    SoDebugError::postInfo("SoFlComponent::setSize",
                           " baseWidget %s",
                           dumpWindowData(this->getBaseWidget()).c_str());
    SoDebugError::postInfo("SoFlComponent::setSize",
                           " shellWidget %s",
                           dumpWindowData(this->getShellWidget()).c_str());
#endif

#if 0
    if ( PRIVATE(this)->embedded ) {
        SoFl::setWidgetSize(this->getBaseWidget(), size);
    }
    else {
        SoFl::setWidgetSize(this->getShellWidget(), size);
    }
#endif
#if 1
#if SOFL_DEBUG
    if((size[0] <= 0) || (size[1] <= 0)) {
        SoDebugError::postWarning("SoFlComponent::setSize",
                                  "Invalid size setting: <%d, %d>.",
                                  size[0], size[1]);
        return;
    }
#endif // SOFL_DEBUG

#if SOFLCOMP_RESIZE_DEBUG  // debug
    SoDebugError::postInfo("SoFlComponent::setSize",
                           "resize %p: (%d, %d)",
                           PRIVATE(this)->widget,
                           size[0], size[1]);
#endif // debug
    const SbBool yetbuilt = (this->getWidget() != nullptr);
    if (yetbuilt) {
        Fl_Window * shell = this->getShellWidget();
        if (shell) {
            shell->size(size[0], size[1]);
        }
    }
#endif
    PRIVATE(this)->storesize = size;
    this->sizeChanged(size);
}

SbVec2s
SoFlComponent::getSize() const {
    return (PRIVATE(this)->storesize);
}

void
SoFlComponent::setTitle(const char * const title) {
    auto shell = SoFl::getShellWidget(this->getWidget());
    if (shell)
        shell->copy_label(title);
    else {
        SoDebugError::postWarning("SoFlComponent::setTitle",
                                  "No shell widget for %p",
                                  this->getWidget());
    }
}

const char *
SoFlComponent::getTitle() const {
    return (SoFl::getShellWidget(this->getWidget())->label());
}

void
SoFlComponent::setIconTitle(const char * const title) {
    SOFL_STUB();
}

const char *
SoFlComponent::getIconTitle() const {
    SOFL_STUB();
    return ("");
}

const char *
SoFlComponent::getWidgetName() const{
    SOFL_STUB();
    return ("");
}

const char *
SoFlComponent::getClassName() const {
    SOFL_STUB();
    return ("");
}

void
SoFlComponent::setWindowCloseCallback(SoFlComponentCB * const func,
                                      void * const user ) {
    SOFL_STUB();
}

void
SoFlComponent::sizeChanged(const SbVec2s & size) {
    SOFL_STUB();
}

void
SoFlComponent::addVisibilityChangeCallback(SoFlComponentVisibilityCB * const func,
                                           void * const user) {
    SOFL_STUB();
}

void
SoFlComponent::removeVisibilityChangeCallback(SoFlComponentVisibilityCB * const func,
                                              void * const user){
    SOFL_STUB();
}

#undef PUBLIC
#undef PRIVATE
