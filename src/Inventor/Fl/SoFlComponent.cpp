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

#include "sowxdefs.h"
#include "SoFlP.h"

#define SOWXCOMP_RESIZE_DEBUG 1

#define PRIVATE(obj) ((obj)->pimpl)
#define PUBLIC(obj) ((obj)->pub)

SOWX_OBJECT_ABSTRACT_SOURCE(SoFlComponent);

SoFlComponent::~SoFlComponent(){

}

SoFlComponent::SoFlComponent(wxWindow* const parent,
                             const char * const name,
                             const SbBool embed) {
    PRIVATE(this) = new SoFlComponentP(this);

    PRIVATE(this)->realized = false;
    PRIVATE(this)->shelled = false;
    PRIVATE(this)->widget = NULL;
    PRIVATE(this)->parent = parent;
    PRIVATE(this)->closeCB = NULL;
    PRIVATE(this)->closeCBdata = NULL;
    PRIVATE(this)->visibilitychangeCBs = NULL;
    PRIVATE(this)->fullscreen = false;

    this->setClassName("SoFlComponent");

    PRIVATE(this)->storesize.setValue(-1, -1);

    SoAny::si()->addInternalFatalErrorHandler(SoFlComponentP::fatalerrorHandler,
                                              PRIVATE(this));

    PRIVATE(this)->widgetname = (name ? name :
                                 this->getDefaultWidgetName());

    if (!parent || !embed) {
        PRIVATE(this)->parent = new wxFrame(NULL, wxID_ANY, name);
        PRIVATE(this)->embedded = false;
        PRIVATE(this)->shelled = true;
    }
    else {
        PRIVATE(this)->parent = parent;
        PRIVATE(this)->embedded = true;
    }

    // TODO: PRIVATE(this)->parent->installEventFilter(PRIVATE(this));
}

void  SoFlComponent::initClasses(void) {
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
SoFlComponent::afterRealizeHook(void) {
    SOWX_STUB();
}

void
SoFlComponent::setClassName(const char * const name) {
    PRIVATE(this)->classname = name;
}

void
SoFlComponent::setBaseWidget(wxWindow* w) {

#ifdef SOWX_DEBUG
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
         */
        widgetName = (PRIVATE(this)->widget->GetName().IsEmpty() ?
                      widgetName :
                      PRIVATE(this)->widget->GetName());

        this->unregisterWidget(PRIVATE(this)->widget);
    }

    PRIVATE(this)->widget = w;

    this->registerWidget(PRIVATE(this)->widget);

#if SOWX_DEBUG && 0
    SoDebugError::postInfo("SoFlComponent::setBaseWidget",
                           "widget: %p, parent: %p", w, PRIVATE(this)->parent);
#endif

    if (!PRIVATE(this)->parent || PRIVATE(this)->parent->IsTopLevel()) {

        if (PRIVATE(this)->widget->GetName() == "") {
            this->setTitle(this->getDefaultTitle());
        }

        SoFl::getShellWidget(this->getWidget())->SetName(iconText);
    }
    PRIVATE(this)->widget->SetName(widgetName);
}

void
SoFlComponent::show(void) {
    if(SOWX_DEBUG && !PRIVATE(this)->widget) { // debug
        SoDebugError::postWarning("SoFlComponent::show",
                                  "Called while no wxWindow has been set.");
        return;
    }

    if (SOWXCOMP_RESIZE_DEBUG) {  // debug
        SoDebugError::postInfo("SoFlComponent::show-1",
                               "resizing %p: (%d, %d)",
                               PRIVATE(this)->widget,
                               PRIVATE(this)->storesize[0],
                               PRIVATE(this)->storesize[1]);
    }

    // only parent has power for setting the correct size
    PRIVATE(this)->parent->SetClientSize(PRIVATE(this)->storesize[0],
                                         PRIVATE(this)->storesize[1]);

    if (SOWXCOMP_RESIZE_DEBUG) {  // debug
        SoDebugError::postInfo("SoFlComponent::show-2",
                               "resized %p: (%d, %d)",
                               PRIVATE(this)->widget,
                               PRIVATE(this)->widget->GetSize().GetX(),
                               PRIVATE(this)->widget->GetSize().GetY());
    }

    if(! PRIVATE(this)->embedded && PRIVATE(this)->shelled) {
#if SOWX_DEBUG
        SoDebugError::postInfo("SoFlComponent::show",
                               "perform show if is not embedded and is shelled");
        PRIVATE(this)->parent->Show();
#endif
    }
    PRIVATE(this)->widget->Show();

    this->sizeChanged(PRIVATE(this)->storesize);
}

void
SoFlComponent::hide(void) {
    SOWX_STUB();
    PRIVATE(this)->widget->Hide();
}

void
SoFlComponent::setComponentCursor(const SoFlCursor & cursor) {
    SOWX_STUB();
    SoFlComponent::setWidgetCursor(this->getWidget(), cursor);
}

void
SoFlComponent::setWidgetCursor(wxWindow* w, const SoFlCursor & cursor) {

    if(!w) {
        return;
    }
    // FIXME: as this function is called all the time when the cursor is
    // grabbed by the window under X11, we should really compare with
    // the previous cursor before doing anything, to avoid spending
    // unnecessary clockcycles during animation. 20011203 mortene.

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
}

SbBool
SoFlComponent::isFullScreen(void) const {
    return (PRIVATE(this)->fullscreen);
}

SbBool
SoFlComponent::setFullScreen(const SbBool onoff) {
    wxWindow * w = this->getShellWidget();
    if (w == NULL) w = this->getParentWidget();
    if (w == NULL) w = this->getWidget();
    if (!w) { return false; }
    wxFrame* frame =  dynamic_cast<wxFrame*>(w);
    if(frame) {
        frame->ShowFullScreen(onoff);
    }
    else {
        return (false);
    }

    PRIVATE(this)->fullscreen = onoff;
    return (PRIVATE(this)->fullscreen);
}

SbBool
SoFlComponent::isVisible(void) {
    bool ret = false;
    if( PRIVATE(this)->widget ) {
        ret = PRIVATE(this)->widget->IsShownOnScreen();
    }
    return (ret);
}

SbBool
SoFlComponent::isTopLevelShell(void) const {
#if SOWX_DEBUG
    if (! PRIVATE(this)->widget) {
        SoDebugError::postWarning("SoFlComponent::isTopLevelShell",
                                  "Called while no wxWindow has been set.");
        return false;
    }
#endif // SOWX_DEBUG
    return PRIVATE(this)->embedded ? false : true;
}

wxWindow*
SoFlComponent::getWidget(void) const {
    return this->getBaseWidget();
}

wxWindow*
SoFlComponent::getBaseWidget(void) const {
    return PRIVATE(this)->widget;
}

wxWindow*
SoFlComponent::getParentWidget(void) const {
    return PRIVATE(this)->parent;
}

void
SoFlComponent::setSize(const SbVec2s size) {

#ifdef SOWX_DEBUG
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
#if SOWX_DEBUG
    if((size[0] <= 0) || (size[1] <= 0)) {
        SoDebugError::postWarning("SoFlComponent::setSize",
                                  "Invalid size setting: <%d, %d>.",
                                  size[0], size[1]);
        return;
    }
#endif // SOWX_DEBUG

#if SOWXCOMP_RESIZE_DEBUG  // debug
    SoDebugError::postInfo("SoFlComponent::setSize",
                           "resize %p: (%d, %d)",
                           PRIVATE(this)->widget,
                           size[0], size[1]);
#endif // debug
    const SbBool yetbuilt = (this->getWidget() != NULL);
    if (yetbuilt) {
        wxWindow * shell = this->getShellWidget();
        if (shell) {
            // shell->SetSize(size[0], size[1]);
            shell->SetClientSize(wxSize(size[0], size[1]));
        }
    }
#endif
    PRIVATE(this)->storesize = size;
    this->sizeChanged(size);
}

SbVec2s
SoFlComponent::getSize(void) const {
    return (PRIVATE(this)->storesize);
}

void
SoFlComponent::setTitle(const char * const title) {
    SoFl::getShellWidget(this->getWidget())->SetName(title);
}

const char *
SoFlComponent::getTitle(void) const {
    return (SoFl::getShellWidget(this->getWidget())->GetName());
}

void
SoFlComponent::setIconTitle(const char * const title) {
    SOWX_STUB();
}

const char *
SoFlComponent::getIconTitle(void) const {
    SOWX_STUB();
    return ("");
}

const char *
SoFlComponent::getWidgetName(void) const{
    SOWX_STUB();
    return ("");
}

const char *
SoFlComponent::getClassName(void) const {
    SOWX_STUB();
    return ("");
}

void
SoFlComponent::setWindowCloseCallback(SoFlComponentCB * const func,
                                      void * const user ) {
    SOWX_STUB();
}

void
SoFlComponent::sizeChanged(const SbVec2s & size) {
    SOWX_STUB();
}

void
SoFlComponent::addVisibilityChangeCallback(SoFlComponentVisibilityCB * const func,
                                           void * const user) {
    SOWX_STUB();
}

void
SoFlComponent::removeVisibilityChangeCallback(SoFlComponentVisibilityCB * const func,
                                              void * const user){
    SOWX_STUB();
}

#undef PUBLIC
#undef PRIVATE
