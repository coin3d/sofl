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

#include "Inventor/Fl/SoFlGLWidget.h"
#include "Inventor/Fl/SoFlGLWidgetP.h"
#include "Inventor/Fl/widgets/SoFlGLArea.h"
#include "sowxdefs.h"
#include "Inventor/Fl/SoAny.h"

SOWX_OBJECT_ABSTRACT_SOURCE(SoFlGLWidget);

#define PRIVATE(obj) ((obj)->pimpl)
#define PUBLIC(obj) ((obj)->pub)

SoFlGLWidget::SoFlGLWidget(wxWindow* const parent ,
                           const char * const name ,
                           const SbBool embed ,
                           const int glmodes ,
                           const SbBool build):
        inherited(parent, name, embed),
        waitForExpose(true),
        drawToFrontBuffer(false) {

    PRIVATE(this) = new SoFlGLWidgetP(this);
    PRIVATE(this)->initGLModes(glmodes);

    PRIVATE(this)->glSize = SbVec2s(0, 0);
    PRIVATE(this)->glSizeUnscaled = SbVec2s(0, 0);
    PRIVATE(this)->wasresized = false;

    PRIVATE(this)->glparent = NULL;
    PRIVATE(this)->currentglwidget = NULL;
    PRIVATE(this)->previousglwidget = NULL;
    PRIVATE(this)->currentglarea = NULL;
    PRIVATE(this)->previousglarea = NULL;

    if (! build) {
        return;
    }

    this->setClassName("SoFlGLWidget");
    wxWindow* parent_widget = this->getParentWidget();
    wxWindow* widget = this->buildWidget(parent_widget);
    this->setBaseWidget(widget);
}

SoFlGLWidget::~SoFlGLWidget() {

    if (PRIVATE(this)->currentglwidget) {
        SoAny::si()->unregisterGLContext((void *)this);
    }

    delete PRIVATE(this);
}

void
SoFlGLWidget::setBorder(const SbBool enable) {
    SOWX_STUB();
}

void
SoFlGLWidget::setQuadBufferStereo(const SbBool enable) {
    SOWX_STUB();
}

SbBool
SoFlGLWidget::isQuadBufferStereo(void) const {
    return (FALSE);
}

void
SoFlGLWidget::setGLSize(const SbVec2s size){
    if (size == PRIVATE(this)->glSize) return;
#if SOWX_DEBUG && 0
    SoDebugError::postInfo("SoFlGLWidget::setGLSize",
                           "[invoked (%d, %d)]", size[0], size[1]);
#endif // debug
    PRIVATE(this)->glSize = size;
    PRIVATE(this)->glSizeUnscaled = size;
    if (PRIVATE(this)->currentglwidget) {
        int frame = this->isBorder() ? PRIVATE(this)->borderthickness : 0;
        // PRIVATE(this)->currentglwidget->SetSize( size[0], size[1] );
    }
}

template <typename T>
T
toSbVec2(const wxSize& wx_size) {
    return (T(wx_size.GetX(), wx_size.GetY()));
}

SbVec2s
SoFlGLWidget::getGLSize(void) const{
    return (PRIVATE(this)->glSize);
}

float
SoFlGLWidget::getGLAspectRatio(void) const{
    SbVec2f v2(toSbVec2<SbVec2f>(PRIVATE(this)->currentglwidget->GetSize()));
    return ( v2[0] /v2[1] );
}

SbBool
SoFlGLWidget::isRGBMode(void){
    const bool rgb_mode = SoFlGLArea::isGLFeatureAvailable(PRIVATE(this)->gl_attributes,
                                                                WX_GL_RGBA);
#if SOWX_DEBUG && 0
    SoDebugError::postInfo("SoFlGLWidget::isRGBMode",
                           ": %d",
                           rgb_mode);
#endif
    return (rgb_mode);
}

void
SoFlGLWidget::glLockNormal(void){
    assert(PRIVATE(this)->currentglarea != NULL);
    PRIVATE(this)->currentglarea->makeCurrent();
}

void
SoFlGLWidget::glUnlockNormal(void){
    // do nothing
}

void
SoFlGLWidget::glLockOverlay(void){
    SOWX_STUB();
}

void
SoFlGLWidget::glUnlockOverlay(void){
    SOWX_STUB();
}

void
SoFlGLWidget::glSwapBuffers(void){
    PRIVATE(this)->currentglarea->SwapBuffers();
}

void
SoFlGLWidget::glFlushBuffer(void){
    glFlush();
}

SbBool
SoFlGLWidget::glScheduleRedraw(void){
    SOWX_STUB();
    return (TRUE);
}

SbBool
SoFlGLWidget::isBorder(void) const{
    return (PRIVATE(this)->borderthickness ? true : false);
}

void
SoFlGLWidget::setDoubleBuffer(const SbBool enable){
    SOWX_STUB();
}

SbBool
SoFlGLWidget::isDoubleBuffer(void) const{
    const bool double_buffer = SoFlGLArea::isGLFeatureAvailable(PRIVATE(this)->gl_attributes,
                                                                WX_GL_DOUBLEBUFFER);
#if SOWX_DEBUG && 0
    SoDebugError::postInfo("SoFlGLWidget::isDoubleBuffer",
                           ": %d",
                           double_buffer);
#endif
    return (double_buffer);
}

void
SoFlGLWidget::setDrawToFrontBufferEnable(const SbBool enable){
    drawToFrontBuffer = enable;
}

SbBool
SoFlGLWidget::isDrawToFrontBufferEnable(void) const{
    return (drawToFrontBuffer);
}

void
SoFlGLWidget::setAccumulationBuffer(const SbBool enable){
    SOWX_STUB();
}

SbBool
SoFlGLWidget::getAccumulationBuffer(void) const{
    SOWX_STUB();
    return (FALSE);
}

void
SoFlGLWidget::setStencilBuffer(const SbBool enable){
    SOWX_STUB();
}

SbBool
SoFlGLWidget::getStencilBuffer(void) const{
    SOWX_STUB();
    return (FALSE);
}

void
SoFlGLWidget::setAlphaChannel(const SbBool enable){
    SOWX_STUB();
}

SbBool
SoFlGLWidget::getAlphaChannel(void) const{
    SOWX_STUB();
    return (FALSE);
}

void
SoFlGLWidget::setOverlayRender(const SbBool onoff){
    SOWX_STUB();
}

SbBool
SoFlGLWidget::isOverlayRender(void) const{
    SOWX_STUB();
    return (FALSE);
}

void
SoFlGLWidget::setSampleBuffers(const int numsamples){
    SOWX_STUB();
}

int
SoFlGLWidget::getSampleBuffers(void) const{
    SOWX_STUB();
    return (0);
}

wxWindow*
SoFlGLWidget::getGLWidget(void) const{
    return (PRIVATE(this)->currentglwidget);
}

wxWindow*
SoFlGLWidget::getNormalWidget(void) const{
    SOWX_STUB();
    return (0);
}

wxWindow*
SoFlGLWidget::getOverlayWidget(void) const{
    SOWX_STUB();
    return (0);
}

SbBool
SoFlGLWidget::hasOverlayGLArea(void) const{
    SOWX_STUB();
    return (false);
}

SbBool
SoFlGLWidget::hasNormalGLArea(void) const{
    bool res = (PRIVATE(this)->currentglarea->context() != 0);
    return (res);
}

unsigned long
SoFlGLWidget::getOverlayTransparentPixel(void){
    SOWX_STUB();
    return (0);
}

void
SoFlGLWidget::processEvent(wxEvent& WXUNUSED(event)){
    // Nothing is done here for the SoFlGLWidget, as realize, resize and
    // expose events are caught by explicitly attaching signal callbacks
    // to the widget.
}

wxWindow *
SoFlGLWidget::buildWidget(wxWindow* parent) {
    assert(parent !=0 && "parent can not be null");
    PRIVATE(this)->glparent = parent;
    return (PRIVATE(this)->buildGLWidget());
}

void
SoFlGLWidget::redrawOverlay(void){
    SOWX_STUB();
}

void
SoFlGLWidget::initGraphic(void){
    this->glLockNormal();
    // Need to set this explicitly when running on top of Open Inventor,
    // as it seems to have been forgotten there.
    // This code should be invoked from SoFlRenderArea::initGraphics()
    glEnable(GL_DEPTH_TEST);
    this->glUnlockNormal();
}
void
SoFlGLWidget::initOverlayGraphic(void){
    SOWX_STUB();
}

void
SoFlGLWidget::sizeChanged(const SbVec2s & size){
    SOWX_STUB();
#if SOWX_DEBUG
    SoDebugError::postInfo("SoFlGLWidget::sizeChanged",
                           "<%d, %d>",
                           size[0], size[1]
    );
#endif
    // Do nothing
}

void
SoFlGLWidget::widgetChanged(wxWindow* w){
    SOWX_STUB();
}

#undef PRIVATE
#undef PUBLIC
