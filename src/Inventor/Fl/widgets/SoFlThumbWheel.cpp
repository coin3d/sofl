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

#include <FL/Fl_Pixmap.H>
#include <FL/Fl_Box.H>

#include <cassert>
#include <iostream>
#include <sstream>
#include <vector>

constexpr short VERTICAL_WIDTH = 24;
constexpr short VERTICAL_HEIGHT = 128;
constexpr short HORIZONTAL_WIDTH = 128;
constexpr short HORIZONTAL_HEIGHT = 24;

static const int SHADEBORDERWIDTH = 0;

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
    box = new Fl_Box(0, 0, this->w(), this->h());
    this->end();
}

#if 0
void
SoFlThumbWheel::paintEvent(int event)
{



#if 0
wxPaintDC dc(this);

int w, dval;
wxSize size = this->GetSize();
    if (this->orient== Vertical) {
        w = size.GetX() - 12;
        dval = size.GetY() - 6;
    } else {
        w = size.GetY() - 12;
        dval = size.GetX() - 6;
    }

#if SOFL_DEBUG
SoDebugError::postInfo ("SoFlThumbWheel::paintEvent",
                           "dval: %d and w: %d",
dval, w);
#endif

// Handle resizing to too small dimensions gracefully.
if ((dval<= 0) || (w<= 0)) return;

    this->initWheel(dval, w);

int pixmap = this->wheel->getBitmapForValue(this->tempWheelValue,
                                            (this->state == SoFlThumbWheel::Disabled)
                                                ? SoAnyThumbWheel::DISABLED
                                                : SoAnyThumbWheel::ENABLED);
#if SOFL_DEBUG
SoDebugError::postInfo ("SoFlThumbWheel::paintEvent",
                           "pixmap value is: %d and bitmap pointer is %p",
pixmap, this->pixmaps);
#endif

if (pixmap>= numPixmaps)
        return;

wxRect widgetrect(0, 0,
                  this->GetSize().GetWidth(),
                  this->GetSize().GetHeight());
wxRect wheelrect(widgetrect);

    if (this->orient== Vertical) {
        wheelrect.SetTop(wheelrect.GetTop() + 2);
        wheelrect.SetBottom(wheelrect.GetBottom() - 4);
        wheelrect.SetLeft(wheelrect.GetLeft() + 5);
        wheelrect.SetRight(wheelrect.GetRight() - 10);
    } else {
        wheelrect.SetTop(wheelrect.GetTop() + 5);
        wheelrect.SetBottom(wheelrect.GetBottom() - 10);
        wheelrect.SetLeft(wheelrect.GetLeft() + 2);
        wheelrect.SetRight(wheelrect.GetRight() - 4);
    }
dc.DrawRectangle (wheelrect);

wheelrect.SetTop (wheelrect.GetTop()+ 1);
wheelrect.SetBottom (wheelrect.GetBottom()- 1);
wheelrect.SetLeft (wheelrect.GetLeft()+ 1);
wheelrect.SetRight (wheelrect.GetRight()- 1);
// wheelrect is now wheel-only

wxRect dRect;
    if (this->orient== Vertical) {
        dRect = wxRect(wheelrect.GetLeft(), wheelrect.GetTop(), w, dval);
    } else {
        dRect = wxRect(wheelrect.GetLeft(), wheelrect.GetTop(), dval, w);
    }

assert (pixmap<numPixmaps);
wxBitmap bitmap(*this->pixmaps[pixmap]);
dc.DrawBitmap(bitmap, dRect.GetX(), dRect.GetY(), false);

    this->currentPixmap= pixmap;
#endif

}

/*!
  \internal
*/

void
SoFlThumbWheel::mousePressEvent(int event)
{



#if 0
if (this->state!= SoFlThumbWheel::Idle)
        return;
    this->state= SoFlThumbWheel::Dragging;

    if (this->orient== Vertical)
        this->mouseDownPos= event.GetY()- SHADEBORDERWIDTH- 6;
    else
        this->mouseDownPos= event.GetX()- SHADEBORDERWIDTH- 6;

    this->mouseLastPos=this->mouseDownPos;

#if SOFL_DEBUG
SoDebugError::postInfo ("SoFlThumbWheel::mouseMoveEvent", "");
#endif

sendEvent(SO_WX_MOUSE_WHEEL_PRESSED, "mousePressEvent");
#endif

}

/*!
  \internal
*/
void
SoFlThumbWheel::mouseMoveEvent(int event)
{



#if 0
if (this->state!= SoFlThumbWheel::Dragging)
        return;

    if (this->orient== Vertical)
        this->mouseLastPos= event.GetY()- SHADEBORDERWIDTH- 6;
    else
        this->mouseLastPos= event.GetX()- SHADEBORDERWIDTH- 6;

int delta = this->mouseLastPos - this->mouseDownPos;
#if SOFL_DEBUG
SoDebugError::postInfo ("SoFlThumbWheel::mouseMoveEvent",
                           "delta: %d wheelValue: %d mouseDownPos: %d",
delta,
                           this->wheelValue,
                           this->mouseDownPos);
#endif

this->tempWheelValue=this->wheel->calculateValue (this->wheelValue,
                                                       this->mouseDownPos,
delta);

sendEvent(SO_WX_MOUSE_WHEEL_MOVED, "mouseWheel");
Refresh();
#endif

}

/*!
  \internal
*/
void
SoFlThumbWheel::mouseReleaseEvent(int event)
{



#if 0
if (this->state!= SoFlThumbWheel::Dragging)
        return;

    this->wheelValue=this->tempWheelValue;
    this->mouseLastPos=this->mouseDownPos;
    this->state= SoFlThumbWheel::Idle;
sendEvent(SO_WX_MOUSE_WHEEL_RELEASED, "mouseReleaseEvent");
}

void
SoFlThumbWheel::mouseWheel(int WXUNUSED(event))
{
    SOFL_STUB();
    return;
#if 0
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
#endif
#endif
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
    constexpr int length = 122;
    short thick = 24;

    if (this->orient == SoFlThumbWheel::Horizontal)
        return {length, thick};
    return {thick, length};
}

void SoFlThumbWheel::draw()
{
    int w = this->w();
    int h = this->h();
    int dval{};
    if (this->orient == Vertical)
    {
        w = w - 12;
        dval = h - 6;
    }
    else
    {
        w = h - 12;
        dval = w - 6;
    }

#if SOFL_DEBUG
    SoDebugError::postInfo("SoFlThumbWheel::paintEvent",
                           "dval: %d and w: %d",
                           dval, w);
#endif

    // Handle resizing to too small dimensions gracefully.
    if ((dval <= 0) || (w <= 0)) return;

    this->initWheel(dval, w);

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

    box->image(this->pixmaps[pixmap]);
    box->show();
    this->currentPixmap = pixmap;

    Fl_Window::draw();
}

int SoFlThumbWheel::handle(int event)
{
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
    auto out = static_cast<uint8_t*>(malloc(vout.size()));
    memcpy(out, &vout[0], vout.size());
    return (out);
}

//*/

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
    this->pixmaps = new Fl_RGB_Image*[this->numPixmaps];

    for (int i = 0; i < this->numPixmaps; i++)
    {
        std::vector<unsigned int> buffer(pwidth * pheight);
        this->wheel->drawBitmap(i,
                                &buffer[0],
                                (this->orient == Vertical) ? SoAnyThumbWheel::VERTICAL : SoAnyThumbWheel::HORIZONTAL);

        std::cerr << "sizeof:" << sizeof(unsigned int) << std::endl;
        uint8_t* rgb = toRGBChannel(buffer);
        this->pixmaps[i] = new Fl_RGB_Image(rgb, buffer.size() * 32, pwidth, pheight, 3, 36);
        std::cerr << "ld:" << this->pixmaps[i]->ld() << std::endl;
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
    // Refresh();
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
