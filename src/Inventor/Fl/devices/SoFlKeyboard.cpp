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

#include <Inventor/SbTime.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/errors/SoDebugError.h>

#include "Inventor/Fl/devices/SoFlKeyboard.h"
#include "SoFlKeyboardP.h"
#include "sofldefs.h"

#define PRIVATE(p) (p->pimpl)
#define PUBLIC(p) (p->pub)

// *************************************************************************


SoFlKeyboard::SoFlKeyboard(int eventmask) {
    PRIVATE(this) = new SoFlKeyboardP;
    PRIVATE(this)->eventmask = eventmask;
}

SoFlKeyboard::~SoFlKeyboard(void) {
    delete PRIVATE(this);
}

void
SoFlKeyboard::enable(Fl_Window *widget, SoFlEventHandler *handler, void *closure) {
    SOFL_STUB();
}

void
SoFlKeyboard::disable(Fl_Window *widget, SoFlEventHandler *handler, void *closure) {
    SOFL_STUB();
}

const SoEvent *
SoFlKeyboard::translateEvent(int event) {
    int key_event = event;

    if (key_event == FL_KEYBOARD) {
#ifdef SOFL_DEBUG
    SoDebugError::postWarning("SoFlKeyboard::translateEvent",
                              "key event!");
#endif
    return (0);
    }
#if 0
    SbBool keypress = key_event->GetEventType() == wxEVT_KEY_DOWN;
    SbBool keyrelease = key_event->GetEventType() == wxEVT_KEY_UP;

    SbBool keyevent = keypress || keyrelease;

    if (keyevent && (PRIVATE(this)->eventmask & (KEY_PRESS | KEY_RELEASE))) {
        if (!SoFlKeyboardP::translatetable)
            SoFlKeyboardP::make_translation_table();

        int key = key_event->GetKeyCode();
        // Key code / sequence unknown.
        if (key == 0) return nullptr;

        // Allocate system-neutral event object once and reuse.
        if (!PRIVATE(this)->kbdevent) PRIVATE(this)->kbdevent = new SoKeyboardEvent;

        PRIVATE(this)->kbdevent->setPrintableCharacter(key);

        // Translate keycode fl -> So
        void *table;
        if (SoFlKeyboardP::translatetable->find(key, table)) {
            struct SoFlKeyboardP::key1map *map = (struct SoFlKeyboardP::key1map *) table;
            PRIVATE(this)->kbdevent->setKey(map->to);
        } else {
            return nullptr;
        }

        // Press or release?
        if (keyrelease) PRIVATE(this)->kbdevent->setState(SoButtonEvent::UP);
        else
            PRIVATE(this)->kbdevent->setState(SoButtonEvent::DOWN);

        PRIVATE(this)->kbdevent->setShiftDown(key_event->ShiftDown());
        PRIVATE(this)->kbdevent->setCtrlDown(key_event->ControlDown());
        PRIVATE(this)->kbdevent->setAltDown(key_event->AltDown());

        this->setEventPosition(PRIVATE(this)->kbdevent,
                               key_event->GetX(),
                               key_event->GetY());

        PRIVATE(this)->kbdevent->setTime(SbTime::getTimeOfDay());
        return PRIVATE(this)->kbdevent;
    }
#endif
    return (nullptr);
}
