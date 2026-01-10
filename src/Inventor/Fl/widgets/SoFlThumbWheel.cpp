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

\note fltk requires a position for a valid window.
*/


#include "SoFlThumbWheel.h"

#include <Inventor/Fl/widgets/SoAnyThumbWheel.h>
#include <Inventor/errors/SoDebugError.h>

#include <cassert>
#include <vector>
#include <FL/fl_draw.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_RGB_Image.H>
#include <iostream>


#include <FL/Fl_Box.H>
#include <FL/Enumerations.H>
#include <string>
#include <sstream>
#include <iomanip>

std::string get_box_info_string(Fl_Box* box) {
    if (!box) return "Errore: puntatore alla box nullo.";

    std::ostringstream ss;

    // Estrazione componenti RGB del colore di sfondo
    uchar r, g, b;
    Fl::get_color(box->color(), r, g, b);

    ss << "--- INFO FL_BOX ---\n"
       << "Label: " << (box->label() ? box->label() : "(null)") << "\n"
       << "Geometry: x:" << box->x() << " y:" << box->y() << " w:" << box->w() << " h:" << box->h() << "\n"
       << "Color (Hex): 0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << (unsigned int)box->color() << "\n"
       << "Color (RGB): R:" << std::dec << (int)r << " G:" << (int)g << " B:" << (int)b << "\n"
       << "Boxtype ID: " << (int)box->box() << "\n"
       << "Align: " << (int)box->align() << "\n"
       << "Visible: " << (box->visible() ? "Yes" : "No") << "\n"
       << "Active: " << (box->active() ? "Yes" : "No") << "\n"
       << "-------------------";

    return ss.str();
}
namespace
{
    constexpr short VERTICAL_WIDTH = 24;
    constexpr short VERTICAL_HEIGHT = 122;
    constexpr short HORIZONTAL_WIDTH = 122;
    constexpr short HORIZONTAL_HEIGHT = 24;
    constexpr int SHADEBORDERWIDTH = 0;

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
}

SoFlThumbWheel::SoFlThumbWheel(const SbVec2s& pos,
                               const char* name)
    : Fl_Group(pos[0], pos[1],
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
    : Fl_Group(pos[0], pos[1],
                orientation == Horizontal ? HORIZONTAL_WIDTH : VERTICAL_WIDTH,
                orientation == Horizontal ? HORIZONTAL_HEIGHT : VERTICAL_HEIGHT,
                name)
{
    if (!name)
        this->copy_label("SoFlThumbWheel");
    else
        this->copy_label(name);
    this->constructor(orientation);
#if SOFL_DEBUG
    SoDebugError::postInfo("SoFlThumbWheel::SoFlThumbWheel",
                           "<w: %d, h: %d>",
                           w(), h());
#endif
    // not more Fl_Widgets will be added
    this->end();
}

SoFlThumbWheel::~SoFlThumbWheel()
{
    delete this->wheel;
    this->cleanPixmaps();
}

void
SoFlThumbWheel::setOrientation(Orientation orientation)
{
    this->orient = orientation;
    this->redraw();
}

/*!
  \internal
*/
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
    this->fl_images = nullptr;
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

    if (this->state != Idle)
        return;

    this->state = Dragging;

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
#if SOFL_DEBUG && 0
    if (event != 11)
    {
        SoDebugError::postInfo("SoFlThumbWheel::mouseMoveEvent",
                               "event: %d",
                               event);
    }
#endif

    if (this->state != Dragging)
        return;

    if (this->orient == Vertical)
        this->mouseLastPos = Fl::event_y() - SHADEBORDERWIDTH - 6;
    else
        this->mouseLastPos = Fl::event_x() - SHADEBORDERWIDTH - 6;

    int delta = this->mouseLastPos - this->mouseDownPos;

    this->tempWheelValue = this->wheel->calculateValue(this->wheelValue,
                                                       this->mouseDownPos,
                                                       delta);

#if SOFL_DEBUG && 0
    SoDebugError::postInfo("SoFlThumbWheel::mouseMoveEvent",
                           "delta: %d <x,y>: %d,%d tempWheelValue: %d mouseDownPos: %d",
                           delta,
                           Fl::event_x(), Fl::event_y(),
                           this->tempWheelValue,
                           this->mouseDownPos);
#endif
    redraw();
}

/*!
  \internal
*/
void
SoFlThumbWheel::mouseReleaseEvent(int event)
{
#if SOFL_DEBUG && 0
    SoDebugError::postInfo("SoFlThumbWheel::mouseReleaseEvent",
                           "event: %d",
                           event);
#endif

    if (this->state != Dragging)
        return;

    this->wheelValue = this->tempWheelValue;
    this->mouseLastPos = this->mouseDownPos;
    this->state = Idle;
    redraw();
}

SbVec2s
SoFlThumbWheel::sizeHint() const
{
    constexpr short length = HORIZONTAL_WIDTH;
    constexpr short thick = HORIZONTAL_HEIGHT;

    if (this->orient == Horizontal)
        return {length, thick};
    return {thick, length};
}

/*!
  \internal
*/
void
SoFlThumbWheel::draw()
{
    int w_val{};
    int dval{};
#if SOFL_DEBUG
    SoDebugError::postInfo("SoFlThumbWheel::draw",
                           "orientation: %s <w,h>: <%d,%d>",
                           orient == Vertical ? "Vertical" : "Horizontal",
                           this->w(), this->h());
#endif
    if (this->orient == Vertical)
    {
        w_val = this->w() - 12;
        dval = this->h() - 6;
    }
    else
    {
        w_val = this->h() - 12;
        dval = this->w() - 6;
    }

    // Handle resizing to too small dimensions gracefully.
    if ((dval <= 0) || (w_val <= 0)) return;

    this->initWheel(dval, w_val);

    int pixmap = this->wheel->getBitmapForValue(this->tempWheelValue,
                                                (this->state == Disabled)
                                                    ? SoAnyThumbWheel::DISABLED
                                                    : SoAnyThumbWheel::ENABLED);

    if (pixmap >= numPixmaps)
        return;
    assert(pixmap < numPixmaps);

    this->fl_images[pixmap]->draw(x(), y());

    this->currentPixmap = pixmap;
    Fl_Group::draw();
}

/*!
  \internal
*/
int
SoFlThumbWheel::handle(int event)
{
#if SOFL_DEBUG && 0
    SoDebugError::postInfo("SoFlThumbWheel::handle",
                           "event: %d",
                           event);
#endif
    switch (event)
    {
    case FL_PUSH:
        this->mousePressEvent(event);
        return 1;
    case FL_RELEASE:
        this->mouseReleaseEvent(event);
        return 1;
    case FL_DRAG:
    case FL_MOVE:
        this->mouseMoveEvent(event);
        return 1;
    case FL_ENTER:
    case FL_KEYBOARD:
    case FL_LEAVE:
        return 1;
    default:
        break;
    }
    return Fl_Group::handle(event);
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

void
SoFlThumbWheel::initWheel(int diameter, int width)
{
    if (diameter <= 0 || width <= 0) return;
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

    this->cleanPixmaps();

    this->numPixmaps = this->wheel->getNumBitmaps();
    this->pixmaps = new uint8_t*[this->numPixmaps];
    this->fl_images = new Fl_RGB_Image*[this->numPixmaps];

    for (int i = 0; i < this->numPixmaps; i++)
    {
        auto buffer = new uint8_t [pwidth * pheight *4];
        this->wheel->setGraphicsByteOrder(SoAnyThumbWheel::RGBA);
        this->wheel->drawBitmap(i,
                                buffer,
                                (this->orient == Vertical) ?
                                SoAnyThumbWheel::VERTICAL : SoAnyThumbWheel::HORIZONTAL);

        this->pixmaps[i] = buffer;
        this->fl_images[i] = new Fl_RGB_Image(buffer, pwidth, pheight, 4);
    }
}

void
SoFlThumbWheel::cleanPixmaps()
{
    for (int i = 0; i < this->numPixmaps; i++)
    {
        delete this->fl_images[i];
        delete this->pixmaps[i];
    }
    delete [] this->fl_images;
    delete [] this->pixmaps;
    this->numPixmaps = 0;
    this->fl_images = nullptr;
    this->pixmaps = nullptr;
}


void
SoFlThumbWheel::setEnabled(bool enable)
{
    if (enable)
        this->state = Idle;
    else
        this->state = Disabled;
    this->redraw();
}

bool
SoFlThumbWheel::isEnabled() const
{
    return (this->state != Disabled);
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
}
