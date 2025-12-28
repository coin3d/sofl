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

#include "Inventor/Fl/SoFlGLWidgetP.h"
#include "Inventor/Fl/SoFlGLWidget.h"
#include "Inventor/Fl/widgets/SoFlGLArea.h"
#include "Inventor/Fl/SoAny.h"

#include <Inventor/SbTime.h>

#include "sofldefs.h"

#define PRIVATE(obj) ((obj)->pimpl)
#define PUBLIC(obj) ((obj)->pub)

SoFlGLWidgetP::SoFlGLWidgetP(SoFlGLWidget * o)
        : SoGuiGLWidgetP(o) {
    this->borderthickness = 0;
    this->oldcontext = nullptr;
}

SoFlGLWidgetP::~SoFlGLWidgetP() {
}

void
SoFlGLWidgetP::gl_init(int )
{
#if SOFL_DEBUG
    SoDebugError::postInfo("gl_init", "invoked");
#endif

    PUBLIC(this)->initGraphic();
}

void
SoFlGLWidgetP::gl_reshape(int event) {
#if SOFL_DEBUG
    SoDebugError::postInfo("SoFlGLWidgetP::gl_reshape",
                           "<%d, %d>",
                           currentglwidget->w(),
                           currentglwidget->h());
#endif

    this->glSize = SbVec2s( currentglarea->w(), currentglarea->h());
    this->wasresized = true;
    pub->setSize(this->glSize);
}

void
SoFlGLWidgetP::gl_exposed(int) {
#if SOFL_DEBUG
    SoDebugError::postInfo("SoFlGLWidgetP::gl_exposed", "%f", SbTime::getTimeOfDay().getValue());
#endif

    if (PUBLIC(this)->waitForExpose) {
#if SOFL_DEBUG
        SoDebugError::postInfo("SoFlGLWidgetP::gl_exposed", "waitForExpose");
#endif
        PUBLIC(this)->waitForExpose = false; // Gets flipped from TRUE on first expose.
        PUBLIC(this)->setSize(PUBLIC(this)->getSize());
    }
    if (this->wasresized) {
#if SOFL_DEBUG
        SoDebugError::postInfo("SoFlGLWidgetP::gl_exposed", "wasresized");
#endif
        PUBLIC(this)->sizeChanged(this->glSize);
        this->wasresized = false;
    }

    if (!PUBLIC(this)->glScheduleRedraw()) {
        PUBLIC(this)->redraw();
    }
}

static const char eventnaming[][50] = {
        "None", // 0
        "Timer",
        "MouseButtonPress",
        "MouseButtonRelease",
        "MouseButtonDblClick",
        "MouseMove",
        "KeyPress",
        "KeyRelease",
        "FocusIn",
        "FocusOut",
        "Enter",
        "Leave",
        "Paint",
        "Move",
        "Resize",
        "Create",
        "Destroy",
        "Show",
        "Hide",
        "Close",
        "Quit", // 20
        "*unknown*", "*unknown*", "*unknown*", "*unknown*", "*unknown*",
        "*unknown*", "*unknown*", "*unknown*", "*unknown*",
        "Accel", // 30
        "Wheel",
        "AccelAvailable", // 32
        "CaptionChange",
        "*unknown*", "*unknown*", "*unknown*", "*unknown*", "*unknown*", "*unknown*",
        "Clipboard", // 40
        "*unknown*", "*unknown*", "*unknown*", "*unknown*", "*unknown*",
        "*unknown*", "*unknown*", "*unknown*", "*unknown*",
        "SockAct", // 50
        "AccelOverride", "*unknown*", "*unknown*", "*unknown*", "*unknown*",
        "*unknown*", "*unknown*", "*unknown*", "*unknown*",
        "DragEnter", // 60
        "DragMove",
        "DragLeave",
        "Drop",
        "DragResponse", // 64
        "*unknown*", "*unknown*", "*unknown*", "*unknown*", "*unknown*",
        "ChildInserted", // 70
        "ChildRemoved",
        "LayoutHint", // 72
        "*unknown*", "*unknown*", "*unknown*", "*unknown*", "*unknown*",
        "*unknown*", "*unknown*",
        "ActivateControl", // 80
        "DeactivateControl"
};

// The GL widget rebuilding has been written to remember the previous
// GL widget, which might be swapped back in if it fits the wanted
// format.
//
// There are two reasons for keeping track of both the current and the
// previous widget:
//
//  1) efficiency; often one swaps back and forth between only two
//  different visuals -- like single and double buffering, or stereo
//  mode on and off
//
//  2) robustness; killing off the previous widget in the build-method
//  below has nasty sideeffects (like "random" coredumps), since the
//  event loop might be using it
SoFlGLArea*
SoFlGLWidgetP::buildGLWidget(void) {

    try {
#if SOFL_DEBUG
        SoDebugError::postInfo("SoFlGLWidgetP::buildGLWidget",
                "%s, %s, %s, %s, %s",
                               PUBLIC(this)->isDoubleBuffer() ? "double" : "single",
                               this->hasZBuffer() ? "z-buffer" : "no z-buffer",
                               PUBLIC(this)->isRGBMode() ? "RGBA" : "colorindex",
                               PUBLIC(this)->isQuadBufferStereo() ? "stereo" : "mono",
                               this->hasOverlay() ? "overlay" : "no overlay");
#endif

        Fl_Widget *wascurrent = this->currentglwidget;
        Fl_Widget *wasprevious = this->previousglwidget;
        SoFlGLArea *wascurrentarea = this->currentglarea;
        SoFlGLArea *waspreviousarea = this->previousglarea;

        void *display = nullptr;
        void *screen = nullptr;

        if (wascurrent) {
            // Do _not_ turn off mousetracking or remove the eventfilter, as
            // we'd loose events after the switch has happened if the user is
            // already interacting with the canvas (e.g. when starting a drag
            // in BUFFER_INTERACTIVE mode).
            //TODO: QObject::disconnect(wascurrentarea, SIGNAL(expose_sig()), this, SLOT(gl_exposed()));
            //TODO: QObject::disconnect(wascurrentarea, SIGNAL(init_sig()), this, SLOT(gl_init()));
            //TODO: QObject::disconnect(wascurrentarea, SIGNAL(screenChanged(QScreen*)), this, SLOT(gl_changed()));
            //    QObject::disconnect(wascurrentarea, SIGNAL(reshape_sig()), this, SLOT(gl_reshape()));
            this->previousglwidget = wascurrent;
            this->previousglarea = wascurrentarea;
        }

        if (wasprevious
            // TODO: && QGLFormat_eq(*this->glformat, waspreviousarea->format())
                ) {
            // Reenable the previous widget.
            if (this->currentglwidget) SoAny::si()->unregisterGLContext((void *) PUBLIC(this));
            this->currentglwidget = wasprevious;
            this->currentglarea = waspreviousarea;
            SoAny::si()->registerGLContext((void *) PUBLIC(this), display, screen);
#if SOFL_DEBUG
                SoDebugError::postInfo("SoFlGLWidgetP::buildGLWidget",
                                       "reused previously used GL widget");
#endif
        } else {
            if (this->currentglwidget)
                SoAny::si()->unregisterGLContext((void *) PUBLIC(this));

            this->currentglarea = new SoFlGLArea(
                    glparent,
                    gl_attributes);

            // a wxPanel need a sizer, look at: https://forums.fltk.org/viewtopic.php?t=44252
            if( SoFlGLWidgetP::isAPanel(glparent)) {
                // new sizer remove the previous one (I'm sorry)
                addSizer();
            }

            this->currentglwidget = this->currentglarea;
            // TODO: this->currentglarea->registerQKeyEventHandler(SoFlGLWidgetP::GLAreaKeyEvent, PUBLIC(this));
            // TODO: this->currentglwidget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
            SoAny::si()->registerGLContext((void *) PUBLIC(this), display, screen);
            // Send this one to the final hunting grounds.
            delete wasprevious;
        }

        if (SOFL_DEBUG) { // Warn about requested features that we didn't get.
            // TODO: QSurfaceFormat * w = this->glformat; // w(anted)
            // TODO: QSurfaceFormat g = this->currentglarea->format(); // g(ot)

#define GLWIDGET_FEATURECMP(_glformatfunc_, _cmpvalue_, _truestr_, _falsestr_) \
  do { \
    if (w->_glformatfunc_() != g._glformatfunc_()) { \
      SoDebugError::postWarning("SoFlGLWidgetP::buildGLWidget", \
                                "wanted %s, but that is not supported " \
                                "by the OpenGL driver", \
                                w->_glformatfunc_() == _cmpvalue_ ? _truestr_ : _falsestr_); \
    } \
  } while (0)

            // TODO: GLWIDGET_FEATURECMP(swapBehavior, QSurfaceFormat::DoubleBuffer, "doublebuffer visual", "singlebuffer visual");
            // TODO: GLWIDGET_FEATURECMP(depthBufferSize, 0, "visual without depthbuffer", "visual with depthbuffer");
            // TODO: //GLWIDGET_FEATURECMP(rgba, true, "RGBA buffer", "colorindex buffer");
            // TODO: GLWIDGET_FEATURECMP(stereo, true, "stereo buffers", "mono buffer");

#if HAVE_QT_SAMPLE_BUFFERS
            // TODO:     GLWIDGET_FEATURECMP(samples, 0, "no sample buffers", "sample buffers");
#endif

// TODO:
#if 0
            if (QGLFormat_hasOverlay(w) != QGLFormat_hasOverlay(&g)) {
                SoDebugError::postWarning("SoFlGLWidgetP::buildGLWidget",
                                          "wanted %s, but that is not supported "
                                          "by the OpenGL driver",
                                          QGLFormat_hasOverlay(w) ?
                                          "overlay plane(s)" :
                                          "visual without overlay plane(s)");
            }
#endif
        }
#undef GLWIDGET_FEATURECMP

        //TODO: this->glformat = this->currentglarea->format();

        int frame = PUBLIC(this)->isBorder() ? this->borderthickness : 0;
        // TODO: this->currentglwidget->setGeometry(frame, frame,this->glSize[0] - 2*frame,this->glSize[1] - 2*frame);
/*
        this->currentglarea->Bind(wxEVT_SIZE, &SoFlGLWidgetP::gl_reshape, this);
        this->currentglarea->Bind(SO_WX_GL_INIT, &SoFlGLWidgetP::gl_init, this);
        this->currentglarea->Bind(SO_WX_GL_DRAW, &SoFlGLWidgetP::gl_exposed, this);
        this->currentglarea->Bind(wxEVT_LEFT_DOWN, &SoFlGLWidgetP::onMouse, this);
        this->currentglarea->Bind(wxEVT_RIGHT_DOWN, &SoFlGLWidgetP::onMouse, this);
        this->currentglarea->Bind(wxEVT_LEFT_UP, &SoFlGLWidgetP::onMouse, this);
        this->currentglarea->Bind(wxEVT_RIGHT_UP, &SoFlGLWidgetP::onMouse, this);
        this->currentglarea->Bind(wxEVT_MOTION, &SoFlGLWidgetP::onMouse, this);
        this->currentglarea->Bind(wxEVT_MOUSEWHEEL, &SoFlGLWidgetP::onMouse, this);
        this->currentglarea->Bind(wxEVT_KEY_DOWN, &SoFlGLWidgetP::onKey, this);
        this->currentglarea->Bind(wxEVT_KEY_UP, &SoFlGLWidgetP::onKey, this);
*/
        // Reset to avoid unnecessary scenegraph redraws.
        PUBLIC(this)->waitForExpose = true;

        // We've changed to a new widget, so notify subclasses through this
        // virtual method.
        PUBLIC(this)->widgetChanged(this->currentglwidget);
    }
    catch (std::exception& e) {
        SoDebugError::postWarning("SoFlGLWidgetP::buildGLWidget",
                                  "exception: %s",
                                  e.what());
    }
    catch(...) {
        SoDebugError::postWarning("SoFlGLWidgetP::buildGLWidget",
                                  "unknown exception");
    }
    return (this->currentglarea);
}

// Returns the normal GL context.
const GLContext
SoFlGLWidgetP::getNormalContext(void) {
    SoFlGLArea * w = this->currentglarea;
    if (w) return w->context();
    return nullptr;
}

// Returns the overlay GL context.
const GLContext
SoFlGLWidgetP::getOverlayContext(void) {
    SoFlGLArea * w = this->currentglarea;
    // TODO: if (w) { return QGLWidget_overlayContext(w); }
    return nullptr;
}

SbBool
SoFlGLWidgetP::isDirectRendering(void) {
    SbBool res = FALSE;
    //TODO: if(this->currentglarea && this->currentglarea->GetGLCTXAttrs().x11Direct)
        res = TRUE;
    return (res);
}

void SoFlGLWidgetP::initGLModes(int glmodes) {

    gl_attributes.clear();
    if(glmodes & SO_GL_DOUBLE) {
        //gl_attributes.push_back(WX_GL_DOUBLEBUFFER);
    }
    if(glmodes & SO_GL_ZBUFFER) {
        // 24 bit seems to be ok also on Windows
        //gl_attributes.push_back(WX_GL_DEPTH_SIZE);
        gl_attributes.push_back(24);
    }
    if(glmodes & SO_GL_RGB) {
        //gl_attributes.push_back(WX_GL_RGBA);
    }
    if(glmodes & SO_GL_STEREO) {
        //gl_attributes.push_back(WX_GL_STEREO);
    }
    gl_attributes.push_back(0);

    /*
    if(!SoFlGLArea::IsDisplaySupported(&gl_attributes[0])) {
        SoDebugError::postInfo("SoFlGLWidget::SoFlGLWidget",
                               "required GL modes are not supported!");
    }
     */
}

void
SoFlGLWidgetP::eventHandler(Fl_Widget * /*widget*/ , void *closure, int event, bool *) {
#if SOFL_DEBUG
    SoDebugError::postInfo("SoFlGLWidgetP::eventHandler",
                           "");
#endif
    assert(closure != nullptr);
    SoFlGLWidget * component = ((SoFlGLWidgetP *) closure)->pub;
    component->processEvent(event);
}

void
SoFlGLWidgetP::onMouse(int event) {
#if SOFL_DEBUG && 0
    SoDebugError::postInfo("SoFlGLWidgetP::onMouse",
                           "mouse event");
#endif
    PUBLIC(this)->processEvent(event);
}

void
SoFlGLWidgetP::onKey(int event) {
#if SOFL_DEBUG
    SoDebugError::postInfo("SoFlGLWidgetP::onKey",
                           "key event");
#endif
    PUBLIC(this)->processEvent(event);
}

bool
SoFlGLWidgetP::isAPanel(Fl_Widget* window) {
    return (false); //window->IsKindOf(wxCLASSINFO(wxPanel)));
}

void
SoFlGLWidgetP::addSizer() {
/*
    if(glparent->GetSizer()) {
        SoDebugError::postWarning("SoFlGLWidgetP::addSizer",
                                  "panel holds a sizer, the old one will be removed");
    }

    wxFlexGridSizer* sizer = new wxFlexGridSizer(0);
    sizer->AddGrowableCol(0);
    sizer->AddGrowableRow(0);
    glparent->SetSizer(sizer);
    sizer->Add(this->currentglarea, 0, wxEXPAND | wxALL, 0);
    sizer->Layout();
    */
}

bool
SoFlGLWidgetP::hasZBuffer() const {
    const bool z_buffer = false; // SoFlGLArea::isGLFeatureAvailable(gl_attributes, WX_GL_DEPTH_SIZE);
    return (z_buffer);
}

bool
SoFlGLWidgetP::hasOverlay() const {
    SOFL_STUB();
    return (false);
}

#undef PRIVATE
#undef PUBLIC
