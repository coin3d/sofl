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
#include <FL/fl_draw.H>
#include <FL/Fl_PNG_Image.H>


#include <FL/Enumerations.H>

namespace
{
    const SbVec2s VERTICAL_SIZE(24,122);
    const SbVec2s HORIZONTAL_SIZE(122,24);
    constexpr int   SHADEBORDERWIDTH = 0;
}


SoFlThumbWheel::SoFlThumbWheel(const SbVec2s& pos,
                               const char* name)
    : Fl_Window(pos[0], pos[1],
                HORIZONTAL_SIZE[0], HORIZONTAL_SIZE[1], name)
{
    this->constructor(Vertical);
#if SOFL_DEBUG && 0
    SoDebugError::postInfo("SoFlThumbWheel::SoFlThumbWheel",
                           "<w: %d, h: %d>",
                           w(), h());
#endif
    this->end();
}

SoFlThumbWheel::SoFlThumbWheel(Orientation orientation,
                               const SbVec2s& pos,
                               const char* name)
    : Fl_Window(pos[0], pos[1],
                orientation == Horizontal ? HORIZONTAL_SIZE[0] : VERTICAL_SIZE[0],
                orientation == Horizontal ? HORIZONTAL_SIZE[1] : VERTICAL_SIZE[1],
                name)
{
    if (!name)
        this->copy_label("SoFlThumbWheel");
    else
        this->copy_label(name);
    this->constructor(orientation);
#if SOFL_DEBUG && 0
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
#if SOFL_DEBUG && 0
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

#if SOFL_DEBUG && 0
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
    if (this->orient == Horizontal)
        return HORIZONTAL_SIZE;
    return VERTICAL_SIZE;
}

/*!
  \internal
*/
void
SoFlThumbWheel::draw()
{
    Fl_Window::draw();

    int w_val{};
    int dval{};
#if SOFL_DEBUG && 0
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

    const int pixmap = this->wheel->getBitmapForValue(this->tempWheelValue,
                                                (this->state == Disabled)
                                                    ? SoAnyThumbWheel::DISABLED
                                                    : SoAnyThumbWheel::ENABLED);

    if (pixmap >= numPixmaps)
        return;
    assert(pixmap < numPixmaps);

    const auto position = this->getPosition(fl_images[pixmap]);
    this->fl_images[pixmap]->draw(position[0], position[1]);
    this->currentPixmap = pixmap;
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

/*!
  \internal
*/
SbVec2s
SoFlThumbWheel::getPosition(const Fl_RGB_Image* currImage) const
{
    // retrieve the correct position of image respect to size of image and current window
    auto width_rgb = currImage->w();
    auto height_rgb = currImage->h();
    auto width_fl = this->w();
    auto height_fl = this->h();
    SbVec2s pos((width_fl-width_rgb)*0.5, (height_fl-height_rgb)*0.5);
    if (pos[0] < 0) pos[0] = 0;
    if (pos[1] < 0) pos[1] = 0;
    return pos;
}