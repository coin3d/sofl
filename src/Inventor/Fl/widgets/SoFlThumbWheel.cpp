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

/*!
  \class SoFlThumbWheel SoFlThumbWheel.h Inventor/Fl/widgets/SoFlThumbWheel.h
  \brief The SoFlThumbWheel class is a UI component for fancy looking thumbwheel controls.

  \ingroup components
*/


#include "SoFlThumbWheel.h"

#include <Inventor/Fl/widgets/SoAnyThumbWheel.h>
#include <Inventor/errors/SoDebugError.h>

#include <cassert>
#include <sstream>
#include <vector>
#include <FL/fl_draw.H>

namespace
{
    constexpr short VERTICAL_WIDTH = 24;
    constexpr short VERTICAL_HEIGHT = 122;
    constexpr short HORIZONTAL_WIDTH = 122;
    constexpr short HORIZONTAL_HEIGHT = 24;
    constexpr int SHADEBORDERWIDTH = 0;
}

SoFlThumbWheel::SoFlThumbWheel(const SbVec2s& pos,
                               const char* name)
    : Fl_Window(pos[0], pos[1],
                HORIZONTAL_WIDTH, HORIZONTAL_HEIGHT, name)
{
    this->constructor(Vertical);
#if SOFL_DEBUG
    SoDebugError::postInfo("SoFlThumbWheel::SoFlThumbWheel",
                           "<w: %d, h: %d>",
                           w(), h());
#endif
}

SoFlThumbWheel::SoFlThumbWheel(Orientation orientation,
                               const SbVec2s& pos,
                               const char* name)
    : Fl_Window(pos[0], pos[1],
                orientation == Horizontal ? HORIZONTAL_WIDTH : VERTICAL_WIDTH,
                orientation == Horizontal ? HORIZONTAL_HEIGHT : VERTICAL_HEIGHT,
                name)
{
    if (!name)
        this->label("SoFlThumbWheel");
    else
        this->label(name);
    this->constructor(orientation);
#if SOFL_DEBUG
    SoDebugError::postInfo("SoFlThumbWheel::SoFlThumbWheel",
                           "<w: %d, h: %d>",
                           w(), h());
#endif
}

SoFlThumbWheel::~SoFlThumbWheel()
{
    delete this->wheel;
    if (this->pixmaps)
    {
        for (int i = 0; i < this->numPixmaps; i++)
            delete this->pixmaps[i];
        delete [] this->pixmaps;
    }
}

void
SoFlThumbWheel::setOrientation(Orientation orientation)
{
    this->orient = orientation;
    this->redraw();
}

void
SoFlThumbWheel::constructor(Orientation orientation)
{
    this->orient = orientation;
    this->state = Idle;
    this->wheelValue = this->tempWheelValue = 0.0f;
    this->wheel = new SoAnyThumbWheel;
    this->wheel->setMovement(SoAnyThumbWheel::UNIFORM);
    this->wheel->setGraphicsByteOrder(SoAnyThumbWheel::ARGB);
    this->pixmaps = nullptr;
    this->numPixmaps = 0;
    this->currentPixmap = -1;
}

/*!
  \internal
*/

void
SoFlThumbWheel::mousePressEvent(int event)
{
#if SOFL_DEBUG
    SoDebugError::postInfo("SoFlThumbWheel::mousePressEvent",
                           "event: %d",
                           event);
#endif

    if (this->state != SoFlThumbWheel::Idle)
        return;

    this->state = SoFlThumbWheel::Dragging;

    if (this->orient == Vertical)
        this->mouseDownPos = Fl::y() - SHADEBORDERWIDTH - 6;
    else
        this->mouseDownPos = Fl::x() - SHADEBORDERWIDTH - 6;

    this->mouseLastPos = this->mouseDownPos;

#if SOFL_DEBUG
    SoDebugError::postInfo("SoFlThumbWheel::mousePressEvent",
                           "state: %d mouseDownPos: %d mouseLastPos: %d",
                           state, mouseDownPos, mouseLastPos);
#endif

    redraw();
}

/*!
  \internal
*/
void
SoFlThumbWheel::mouseMoveEvent(int event)
{
#if SOFL_DEBUG
    if (event != 11)
    {
        SoDebugError::postInfo("SoFlThumbWheel::mouseMoveEvent",
                               "event: %d",
                               event);
    }
#endif

    if (this->state != SoFlThumbWheel::Dragging)
        return;

    if (this->orient == Vertical)
        this->mouseLastPos = Fl::y() - SHADEBORDERWIDTH - 6;
    else
        this->mouseLastPos = Fl::x() - SHADEBORDERWIDTH - 6;

    int delta = this->mouseLastPos - this->mouseDownPos;
#if SOFL_DEBUG
    SoDebugError::postInfo("SoFlThumbWheel::mouseMoveEvent",
                           "delta: %d wheelValue: %d mouseDownPos: %d",
                           delta,
                           this->wheelValue,
                           this->mouseDownPos);
#endif

    this->tempWheelValue = this->wheel->calculateValue(this->wheelValue,
                                                       this->mouseDownPos,
                                                       delta);

    redraw();
}

/*!
  \internal
*/
void
SoFlThumbWheel::mouseReleaseEvent(int event)
{
#if SOFL_DEBUG
    SoDebugError::postInfo("SoFlThumbWheel::mouseReleaseEvent",
                           "event: %d",
                           event);
#endif

    if (this->state != SoFlThumbWheel::Dragging)
        return;

    this->wheelValue = this->tempWheelValue;
    this->mouseLastPos = this->mouseDownPos;
    this->state = SoFlThumbWheel::Idle;
    redraw();
}

#if 0
void
SoFlThumbWheel::mouseWheel(int WXUNUSED(event))
{
    SOFL_STUB();
    return;
    int delta = /*event.GetWheelDelta() * */(float)(event.GetWheelRotation()) / 120.0;
#if SOFL_DEBUG
SoDebugError::postInfo ("SoFlThumbWheel::mouseWheel",
                           "delta: %d wheelValue: %d mouseDownPos: %d",
delta,
                           this->wheelValue,
                           this->mouseDownPos);
#endif

this->tempWheelValue=this->wheel->calculateValue (this->wheelValue,
                                                       this->mouseDownPos,
delta);
Refresh();
sendEvent(SO_WX_MOUSE_WHEEL_MOVED,
          "mouseWheel");
}

/*
float
SoFlThumbWheel::getNormalizedValue(int pos) const
{
  int relativepos = pos - this->mouseDownPos;
  return (float) relativepos / (float)this->getWheelLength() * 2.0f;
}
*/

/*
int
SoFlThumbWheel::getWheelLength() const
{
  return this->orient == Vertical ?
    this->GetSize().GetY() : this->GetSize().GetX();
}
*/

/*
int
SoFlThumbWheel::orientedCoord(const QPoint &p) const
{
  return (this->orient == SoFlThumbWheel::Horizontal) ?  p.x() : p.y();
}
*/
#endif

SbVec2s
SoFlThumbWheel::sizeHint() const
{
    constexpr short length = HORIZONTAL_WIDTH;
    constexpr short thick = HORIZONTAL_HEIGHT;

    if (this->orient == SoFlThumbWheel::Horizontal)
        return {length, thick};
    return {thick, length};
}

void SoFlThumbWheel::draw()
{
    Fl_Window::draw();

    int w{};
    int dval{};
    if (this->orient == Vertical)
    {
        w = this->w() - 12;
        dval = this->h() - 6;
    }
    else
    {
        w = this->h() - 12;
        dval = this->w() - 6;
    }

#if SOFL_DEBUG
    SoDebugError::postInfo("SoFlThumbWheel::paintEvent",
                           "dval: %d and w: %d",
                           dval, w);
#endif

    // Handle resizing to too small dimensions gracefully.
    if ((dval <= 0) || (w <= 0)) return;

    this->initWheel(dval, w);
#if SOFL_DEBUG
    SoDebugError::postInfo("SoFlThumbWheel::paintEvent",
                           "tempWheelValue %f",
                           this->tempWheelValue);
#endif

    int pixmap = this->wheel->getBitmapForValue(this->tempWheelValue,
                                                (this->state == SoFlThumbWheel::Disabled)
                                                    ? SoAnyThumbWheel::DISABLED
                                                    : SoAnyThumbWheel::ENABLED);
#if SOFL_DEBUG
    SoDebugError::postInfo("SoFlThumbWheel::paintEvent",
                           "pixmap value is: %d and bitmap pointer is %p",
                           pixmap, this->pixmaps);
#endif

    if (pixmap >= numPixmaps)
        return;
    assert(pixmap < numPixmaps);

    int image_w = w;
    int image_h = dval;
    if (this->orientation() == SoFlThumbWheel::Horizontal)
    {
        std::swap(image_w, image_h);
    }
    Fl::visual(FL_RGB);

    fl_draw_image(this->pixmaps[pixmap], 0, 0, image_w, image_h, 3, 36);
    this->currentPixmap = pixmap;
}

int SoFlThumbWheel::handle(int event)
{
#if SOFL_DEBUG && 0
    SoDebugError::postInfo("SoFlThumbWheel::handle",
                           "event: %d",
                           event);
#endif
    switch (event)
    {
    case FL_KEYBOARD:
        return 1;
    case FL_PUSH:
        this->mousePressEvent(event);
        break;
    case FL_RELEASE:
        this->mouseReleaseEvent(event);
        break;
    case FL_DRAG:
    case FL_MOVE:
        this->mouseMoveEvent(event);
        return 1;
    case FL_ENTER:
    case FL_LEAVE:
        return 1;
    default:
        break;
    }
    return Fl_Window::handle(event);
}

SoFlThumbWheel::Orientation
SoFlThumbWheel::orientation() const
{
    return this->orient;
}

float
SoFlThumbWheel::value() const
{
    return this->wheelValue;
}

//*
void
fill(std::vector<uint8_t>& buffer,
     unsigned long n,
     int channel = 3)
{
    if (channel > 3)
    {
        buffer.push_back((n >> 24) & 0xFF);
    }
    buffer.push_back((n >> 16) & 0xFF);
    buffer.push_back((n >> 8) & 0xFF);
    buffer.push_back(n & 0xFF);
}

uint8_t*
toRGBChannel(const std::vector<unsigned int>& img)
{
    std::vector<uint8_t> vout;
    for (unsigned int i : img)
    {
        fill(vout, i);
    }
    assert(vout.size() == img.size()*3);
    auto out = new uint8_t [vout.size()];
    memcpy(out, &vout[0], vout.size());
    return (out);
}

void
SoFlThumbWheel::initWheel(int diameter, int width)
{
    int dval, w;
    this->wheel->getSize(dval, w);
    if (dval == diameter && w == width) return;

    this->wheel->setSize(diameter, width);

    int pwidth = width;
    int pheight = diameter;
    if (this->orient == Horizontal)
    {
        pwidth = diameter;
        pheight = width;
    }

    if (this->pixmaps != nullptr)
    {
        for (int i = 0; i < this->numPixmaps; i++)
            delete this->pixmaps[i];
        delete [] this->pixmaps;
    }

    this->numPixmaps = this->wheel->getNumBitmaps();
    this->pixmaps = new uint8_t*[this->numPixmaps];

    for (int i = 0; i < this->numPixmaps; i++)
    {
        std::vector<unsigned int> buffer(pwidth * pheight);
        this->wheel->drawBitmap(i,
                                &buffer[0],
                                (this->orient == Vertical) ? SoAnyThumbWheel::VERTICAL : SoAnyThumbWheel::HORIZONTAL);

        this->pixmaps[i] = toRGBChannel(buffer);
    }
}

void
SoFlThumbWheel::setEnabled(bool enable)
{
    if (enable)
        this->state = SoFlThumbWheel::Idle;
    else
        this->state = SoFlThumbWheel::Disabled;
    this->redraw();
}

bool
SoFlThumbWheel::isEnabled() const
{
    return (this->state != SoFlThumbWheel::Disabled);
}

void
SoFlThumbWheel::setValue(float value)
{
    this->wheelValue = this->tempWheelValue = value;
    this->mouseDownPos = this->mouseLastPos;
    redraw();
}

void
SoFlThumbWheel::setRangeBoundaryHandling(boundaryHandling handling)
{
    switch (handling)
    {
    case CLAMP:
        this->wheel->setBoundaryHandling(SoAnyThumbWheel::CLAMP);
        break;
    case MODULATE:
        this->wheel->setBoundaryHandling(SoAnyThumbWheel::MODULATE);
        break;
    case ACCUMULATE:
        this->wheel->setBoundaryHandling(SoAnyThumbWheel::ACCUMULATE);
        break;
    default:
        assert(0 && "impossible");
    }
}

SoFlThumbWheel::boundaryHandling
SoFlThumbWheel::getRangeBoundaryHandling() const
{
    switch (this->wheel->getBoundaryHandling())
    {
    case SoAnyThumbWheel::CLAMP:
        return CLAMP;
    case SoAnyThumbWheel::MODULATE:
        return MODULATE;
    case SoAnyThumbWheel::ACCUMULATE:
        return ACCUMULATE;
    default:
        assert(0 && "impossible");
    }
    return CLAMP; // never reached
}

void
SoFlThumbWheel::sendEvent(long id,
                          const std::string& event_id)
{
#if SOFL_DEBUG
    SoDebugError::postInfo("SoFlThumbWheel::sendEvent",
                           "id: %d event: %s tempWheelValue: %d",
                           id,
                           event_id.c_str(),
                           this->tempWheelValue);
#endif

    /*
    wxCommandEvent a_wx_event(id, GetId());
    a_wx_event.SetEventObject(this);
    a_wx_event.SetString(event_id);
    a_wx_event.SetClientData(&this->tempWheelValue);
    ProcessWindowEvent(a_wx_event);
    */
}
