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

#include "SoFlKeyboardP.h"
#include <FL/Enumerations.H>

SbDict * SoFlKeyboardP::translatetable = nullptr;

struct SoFlKeyboardP::key1map SoFlKeyboardP::wxToSoMapping[] = {
    {FL_Escape, SoKeyboardEvent::ESCAPE, '.'},
        {FL_Tab, SoKeyboardEvent::TAB, '.'},
        {FL_Back, SoKeyboardEvent::BACKSPACE, '.'},
        {FL_Enter, SoKeyboardEvent::RETURN, '.'},
        {FL_Enter, SoKeyboardEvent::ENTER, '.'},
        {FL_Insert, SoKeyboardEvent::INSERT, '.'},
        {FL_Delete, SoKeyboardEvent::KEY_DELETE, '.'},
        {FL_Pause, SoKeyboardEvent::PAUSE, '.'},
        {FL_Print, SoKeyboardEvent::PRINT, '.'},
        {FL_Home, SoKeyboardEvent::HOME, '.'},
        {FL_End, SoKeyboardEvent::END, '.'},
        {FL_Left, SoKeyboardEvent::LEFT_ARROW, '.'},
        {FL_Up, SoKeyboardEvent::UP_ARROW, '.'},
        {FL_Right, SoKeyboardEvent::RIGHT_ARROW, '.'},
        {FL_Down, SoKeyboardEvent::DOWN_ARROW, '.'},
        {FL_Page_Up, SoKeyboardEvent::PRIOR, '.'},
        {FL_Page_Up, SoKeyboardEvent::PAGE_UP, '.'},
        {FL_Page_Down, SoKeyboardEvent::NEXT, '.'},
        {FL_Page_Down, SoKeyboardEvent::PAGE_DOWN, '.'},

        {FL_SHIFT, SoKeyboardEvent::LEFT_SHIFT, '.'},
        {FL_CONTROL, SoKeyboardEvent::LEFT_CONTROL, '.'},
        {FL_ALT, SoKeyboardEvent::LEFT_ALT, '.'},
        {FL_Caps_Lock, SoKeyboardEvent::CAPS_LOCK, '.'},
        {FL_NUM_LOCK, SoKeyboardEvent::NUM_LOCK, '.'},
        {FL_SCROLL_LOCK, SoKeyboardEvent::SCROLL_LOCK, '.'},

        {FL_F, SoKeyboardEvent::F1, '.'},
#if 0
        {WXK_F2, SoKeyboardEvent::F2, '.'},
        {WXK_F3, SoKeyboardEvent::F3, '.'},
        {WXK_F4, SoKeyboardEvent::F4, '.'},
        {WXK_F5, SoKeyboardEvent::F5, '.'},
        {WXK_F6, SoKeyboardEvent::F6, '.'},
        {WXK_F7, SoKeyboardEvent::F7, '.'},
        {WXK_F8, SoKeyboardEvent::F8, '.'},
        {WXK_F9, SoKeyboardEvent::F9, '.'},
        {WXK_F10, SoKeyboardEvent::F10, '.'},
        {WXK_F11, SoKeyboardEvent::F11, '.'},
        {WXK_F12, SoKeyboardEvent::F12, '.'},
{ , SoKeyboardEvent::SPACE, ' '},
#endif


};

void
SoFlKeyboardP::make_translation_table(void)
{
    assert(SoFlKeyboardP::translatetable == nullptr);
    SoFlKeyboardP::translatetable = new SbDict;

    int i = 0;
    while ( SoFlKeyboardP::wxToSoMapping[i].from != 0) {
        SoFlKeyboardP::translatetable->enter((unsigned long)SoFlKeyboardP::wxToSoMapping[i].from,
                                             (void *)&SoFlKeyboardP::wxToSoMapping[i]);
        i++;
    }
}