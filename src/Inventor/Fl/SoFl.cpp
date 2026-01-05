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

#include "Inventor/Fl/SoFl.h"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Window.H>

#include "Inventor/Fl/SoFlP.h"
#include "Inventor/Fl/SoFlInternal.h"
#include "sofldefs.h"

Fl_Window*
SoFl::init(int& argc,
           char** argv,
           const char* appname,
           const char* classname)
{
    if (!SoFlP::instance()->isInitialized())
    {
        // Call all the code for initializing Coin data
        SoFlP::commonInit();

        // if Fl_Windows is not already created
        if (Fl::first_window() == nullptr)
        {
            SoFlP::instance()->setMainWindow(SoFlP::instance()->buildMainWindow(appname));
            Fl::args(argc, argv);
        }

        assert(SoFlP::instance());

        SoDB::getSensorManager()->setChangedCallback(SoGuiP::sensorQueueChanged,
                                                     nullptr);

        SoFlP::instance()->setInitialize(true);
#ifdef SOFL_DEBUG
        SoDebugError::postInfo("SoFl::init",
                               "%s",
                               dumpWindowData(SoFlP::instance()->getMainWindow()).c_str());
#endif
    }
#if SOFL_DEBUG
    else
    {
        SoDebugError::postWarning("SoFl::init",
                                  "This method should be called only once.");
    }
#endif
    return (SoFlP::instance()->getMainWindow());
}

void
SoFl::init(Fl_Window* w )
{
    if (!SoFlP::instance()->isInitialized())
    {
        // Call all the code for initializing Coin data
        SoFlP::commonInit();

        // if Fl_Windows is not already created
        if (Fl::first_window() == nullptr)
        {
            SoFlP::instance()->setMainWindow(w);
        }

        assert(SoFlP::instance());

        SoDB::getSensorManager()->setChangedCallback(SoGuiP::sensorQueueChanged,
                                                     nullptr);

        SoFlP::instance()->setInitialize(true);
#ifdef SOFL_DEBUG
        SoDebugError::postInfo("SoFl::init",
                               "%s",
                               dumpWindowData(SoFlP::instance()->getMainWindow()).c_str());
#endif
    }
#if SOFL_DEBUG
    else
    {
        SoDebugError::postWarning("SoFl::init",
                                  "This method should be called only once.");
    }
#endif
}


/**
 * mainLoop is required only if an external app is not available
 * (embedded in already existing window).
 */
void
SoFl::mainLoop()
{
    SoFlP::instance()->main_window->end();
    Fl::run();
}


void
SoFl::exitMainLoop()
{
    Fl::program_should_quit();
}

void
SoFl::done()
{
    SoFlP::instance()->finish();
#if 0 // FIXME: These methods exist in TGS Inventor. We should add
    // them, and then call them from here. 20060210 kyrah
    SoInteraction::finish();
    SoNodeKit::finish();
#endif
    SoDB::finish();
}

void
SoFl::show(Fl_Window* const widget)
{
    assert(widget != nullptr && "widget can not be null");
    widget->show();
}

void
SoFl::hide(Fl_Window* const widget)
{
    widget->hide();
}

void
SoFl::createSimpleErrorDialog(Fl_Window* widget,
                              const char* title,
                              const char* string1,
                              const char* string2)
{
    SOFL_STUB();
}

Fl_Window*
getTopLevelWidget()
{
#if SOFL_DEBUG
    SoDebugError::postInfo("SoFl::getTopLevelWidget", "%s", __FUNCTION__);
#endif
    return (Fl::first_window());
}

Fl_Window*
SoFl::getShellWidget(const Fl_Window* /*widget*/)
{
#if SOFL_DEBUG
    SoDebugError::postInfo("SoFl::getShellWidget", "%s", __FUNCTION__);
#endif
    return (Fl::first_window());
}

void
SoFl::setWidgetSize(Fl_Window* const widget, const SbVec2s size)
{
#if SOFL_DEBUG
    SoDebugError::postInfo("SoFl::setWidgetSize", "%s", __FUNCTION__);
#endif
    if (widget)
    {
        widget->size(size[0], size[1]);
    }
#if SOFL_DEBUG
    else
    {
        SoDebugError::postWarning("SoFl::setWidgetSize",
                                  "null widget on setting: <%d, %d>.",
                                  size[0], size[1]);
    }
#endif // SOFL_DEBUG
}


SbVec2s
SoFl::getWidgetSize(const Fl_Window* widget)
{
    SbVec2s size(-1, -1);
    if (widget)
    {
        size[0] = widget->w();
        size[1] = widget->h();
    }
#if SOFL_DEBUG
    else
    {
        SoDebugError::postWarning("SoFl::getWidgetSize",
                                  "null widget");
    }
#endif // SOFL_DEBUG
    return size;
}
