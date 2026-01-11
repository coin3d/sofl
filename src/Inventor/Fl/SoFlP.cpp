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

#include "Inventor/Fl/SoFlP.h"

#include <Inventor/SoDB.h>
#include <Inventor/SbTime.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/sensors/SoSensorManager.h>

#include <FL/Fl.H>
#include <FL/Fl_Window.H>


namespace
{
    void timerQueueTimerCB(void* userdata)
    {
#if SOFL_DEBUG
        SoDebugError::postInfo("timerQueueTimerCB",
                               "processing timer queue");
#endif

        SoDB::getSensorManager()->processTimerQueue();

        // The change callback is _not_ called automatically from
        // SoSensorManager after the process methods, so we need to
        // explicitly trigger it ourselves here.
        SoGuiP::sensorQueueChanged(nullptr);
    }

    // The delay sensor timeout point has been reached, so process the
    // delay queue even though the system is not idle (to avoid
    // starvation).
    void delayTimeoutTimerCB(void* userdata)
    {
#if SOFL_DEBUG
        SoDebugError::postInfo("delayTimeoutTimerCB",
                               "processing delay queue");
#endif

        SoDB::getSensorManager()->processTimerQueue();
        SoDB::getSensorManager()->processDelayQueue(false);

        // The change callback is _not_ called automatically from
        // SoSensorManager after the process methods, so we need to
        // explicitly trigger it ourselves here.
        SoGuiP::sensorQueueChanged(nullptr);
    }
}


SoFlP::SoFlP()
{
    init = false;
    main_window = nullptr;
    is_a_soflp_app = false;
}


Fl_Window*
SoFlP::buildMainWindow(const char* title)
{
    if (!main_window)
    {
        is_a_soflp_app = true;
        auto w = new Fl_Window(640, 480);
        w->copy_label(title);
        return w;
    }
#if SOFL_DEBUG
    else
    {
        SoDebugError::postWarning("SoFlP::build_fl_window",
                                  "Fl_Window already built");
    }
#endif
    return nullptr;
}

void SoFlP::setMainWindow(Fl_Window* mainWindow)
{
    assert(mainWindow != nullptr && "main Window can not be null");
    main_window =  mainWindow;
}

void
SoGuiP::sensorQueueChanged(void*)
{
    SoFlP::sensorQueueChanged();
}

void
SoFlP::sensorQueueChanged()
{
    // We need three different mechanisms to interface Coin sensor
    // handling with fltk event handling, which are:
    //
    // 1. Detect when the application is idle and then empty the
    // delay-queue completely for delay-sensors -- handled by
    // SoFlP::onIdle.
    //
    // 2. Detect when one or more timer-sensors are ripe and trigger
    // those -- handled by timerQueueTimerCB.
    //
    // 3. On the "delay-sensor timeout interval", empty all highest
    // priority delay-queue sensors to avoid complete starvation in
    // continually busy applications -- handled by
    // delayTimeoutTimerCB.

    SoSensorManager* sm = SoDB::getSensorManager();

    // Set up a timer queue timeout if necessary.
    SbTime t;
    if (sm->isTimerSensorPending(t))
    {
        SbTime interval = t - SbTime::getTimeOfDay();

        // We also want to avoid setting it to 0.0, as that has a special
        // semantic meaning: trigger only when the application is idle and
        // the event queue is empty -- which is not what we want to do here.
        //
        // So we clamp it, to a small positive value:
        if (interval.getValue() <= 0.0) { interval.setValue(1.0 / 5000.0); }

#if SOFL_DEBUG
        SoDebugError::postInfo("SoFlP::sensorQueueChanged",
                               "timer sensor pending, interval %f",
                               interval.getValue());
#endif

        // Change the interval of timerQueueTimerCB when the head node of the
        // timer-sensor queue of SoSensorManager changes.
        double timeout = interval.getValue();
        Fl::add_timeout(timeout, timerQueueTimerCB, nullptr);
    }
    // Stop timerQueueTimerCB if the queue is completely empty.
    else if (Fl::has_timeout(timerQueueTimerCB, nullptr))
    {
        Fl::remove_timeout(timerQueueTimerCB, nullptr);
    }


    // Set up idle notification to delay queue processing if necessary.
    if (sm->isDelaySensorPending())
    {
#if SOFL_DEBUG
        // debug
        SoDebugError::postInfo("SoFlP::sensorQueueChanged",
                               "delay sensor pending");
#endif


        if (!Fl::has_timeout(delayTimeoutTimerCB, nullptr))
        {
            const SbTime& delaySensorTimeout = SoDB::getDelaySensorTimeout();
            if (delaySensorTimeout != SbTime::zero())
            {
                double timeout = delaySensorTimeout.getValue();
                Fl::add_timeout(timeout, delayTimeoutTimerCB, nullptr);
            }
        }
    }
    else
    {
        if (Fl::has_timeout(delayTimeoutTimerCB, nullptr))
            Fl::remove_timeout(delayTimeoutTimerCB, nullptr);
    }
}

SoFlP*
SoFlP::instance()
{
    static SoFlP singleton;
    return (&singleton);
}

bool
SoFlP::isInitialized() const
{
    return (init);
}

void
SoFlP::setInitialize(bool i)
{
    init = i;
}

Fl_Window*
SoFlP::getMainWindow() const
{
    return (main_window);
}

void
SoFlP::stopTimers()
{
    Fl::remove_timeout(timerQueueTimerCB, nullptr);
    Fl::remove_timeout(delayTimeoutTimerCB, nullptr);
}

void
SoFlP::finish()
{
#ifdef SOFL_DEBUG
    SoDebugError::postInfo("SoFlP::finish",
                           "remove all internal resources");
#endif

    stopTimers();

    // only if the app is built by SoFl perform exit and cleanup
    if (SoFlP::instance()->is_a_soflp_app)
    {
        Fl::program_should_quit();
    }
}

void
SoFlP::onIdle(int event)
{
#if SOFL_DEBUG
    SoDebugError::postInfo("SoFlP::onIdle",
                           "idle sensor pending");
#endif

    SoDB::getSensorManager()->processTimerQueue();
    SoDB::getSensorManager()->processDelayQueue(true);

    // The change callback is _not_ called automatically from
    // SoSensorManager after the process methods, so we need to
    // explicitly trigger it ourselves here.
    SoGuiP::sensorQueueChanged(nullptr);
}

void
SoFlP::onClose(int event)
{
#if SOFL_DEBUG
    SoDebugError::postInfo("SoFlP::onClose",
                           "clean up in progress!");
#endif

    // turn off timers
    SoFlP::stopTimers();

    // To avoid getting any further invocations of
    // SoGuiP::sensorQueueChanged() (which would re-allocate the timers
    // we destruct below). This could, for instance, happen when
    // decoupling the scenegraph camera, triggering a notification
    // chain through the scenegraph.
    SoDB::getSensorManager()->setChangedCallback(nullptr, nullptr);
}
