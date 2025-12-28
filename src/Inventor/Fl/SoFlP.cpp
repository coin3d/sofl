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

#include "Inventor/Fl/SoFlP.h"

#include <Inventor/SoDB.h>
#include <Inventor/SbTime.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/sensors/SoSensorManager.h>

#include <iostream>
#include <sstream>

class SoFlApp : public  wxApp {
public:

    virtual bool
    OnInit()  {
        if ( !wxApp::OnInit() )
            return false;
        return true;
    }

    virtual void
    CleanUp()  {
#if SOFL_DEBUG
        SoDebugError::postInfo("SoFlApp::CleanUp",
                               "done!");
#endif
    }
};

wxTimer * SoFlP::timerqueuetimer = 0;
wxTimer * SoFlP::delaytimeouttimer = 0;

SoFlP::SoFlP() {
    init = false;
    main_frame = 0;
    main_app = 0;
    is_a_soflp_app = false;
}

void
SoFlP::buildWxApp() {
    if(!main_app) {
        is_a_soflp_app = true;
        setWxApp( new SoFlApp);
    } else if (SOFL_DEBUG){
        SoDebugError::postWarning("SoFlP::buildWxApp",
                               "wxApp already built");
    }
}

void
SoFlP::setWxApp(wxAppConsole* app) {
    main_app = app;
}

void
SoGuiP::sensorQueueChanged(void *) {
    SoFlP::instance()->sensorQueueChanged();
}

class TimerQueueTimer : public wxTimer {
public:
    virtual void
    Notify() {
#if SOFL_DEBUG && 0
            SoDebugError::postInfo("TimerQueueTimer::Notify",
                                   "processing timer queue");
            SoDebugError::postInfo("TimerQueueTimer::Notify",
                                   "is %s",
                                   this->IsRunning() ?
                                   "active" : "inactive");
#endif

        SoDB::getSensorManager()->processTimerQueue();

        // The change callback is _not_ called automatically from
        // SoSensorManager after the process methods, so we need to
        // explicitly trigger it ourselves here.
        SoGuiP::sensorQueueChanged(NULL);
    }
};

// The delay sensor timeout point has been reached, so process the
// delay queue even though the system is not idle (to avoid
// starvation).
class DelayTimeoutTimer : public wxTimer {
public:
    virtual void
    Notify() {
#if SOFL_DEBUG && 0
            SoDebugError::postInfo("DelayTimeoutTimer::Notify",
                                   "processing delay queue");
            SoDebugError::postInfo("DelayTimeoutTimer::Notify", "is %s",
                                   this->IsRunning() ?
                                   "active" : "inactive");
#endif

        SoDB::getSensorManager()->processTimerQueue();
        SoDB::getSensorManager()->processDelayQueue(false);

        // The change callback is _not_ called automatically from
        // SoSensorManager after the process methods, so we need to
        // explicitly trigger it ourselves here.
        SoGuiP::sensorQueueChanged(NULL);
    }
};

void
SoFlP::sensorQueueChanged(void) {
    // We need three different mechanisms to interface Coin sensor
    // handling with fltk event handling, which are:
    //
    // 1. Detect when the application is idle and then empty the
    // delay-queue completely for delay-sensors -- handled by
    // SoFlP::onIdle.
    //
    // 2. Detect when one or more timer-sensors are ripe and trigger
    // those -- handled by SoFlP::timerqueuetimer.
    //
    // 3. On the "delay-sensor timeout interval", empty all highest
    // priority delay-queue sensors to avoid complete starvation in
    // continually busy applications -- handled by
    // SoFlP::delaytimeouttimer.


    // Allocate fl timers on first call.
    SoFlP::initTimers();

    SoSensorManager * sm = SoDB::getSensorManager();
    if (SoFlP::timerqueuetimer) {
        // Set up timer queue timeout if necessary.
        SbTime t;
        if (sm->isTimerSensorPending(t)) {
            SbTime interval = t - SbTime::getTimeOfDay();

            // We also want to avoid setting it to 0.0, as that has a special
            // semantic meaning: trigger only when the application is idle and
            // event queue is empty -- which is not what we want to do here.
            //
            // So we clamp it, to a small positive value:
            if (interval.getValue() <= 0.0) { interval.setValue(1.0 / 5000.0); }

#if SOFL_DEBUG && 0
                SoDebugError::postInfo("SoFlP::sensorQueueChanged",
                    "timersensor pending, interval %f",
                    interval.getValue());
#endif

            // Change interval of timerqueuetimer when head node of the
            // timer-sensor queue of SoSensorManager changes.
            SoFlP::timerqueuetimer->Start((int)interval.getMsecValue(), true);
        }
        // Stop timerqueuetimer if queue is completely empty.
        else if (SoFlP::timerqueuetimer->IsRunning()) {
            SoFlP::timerqueuetimer->Stop();
        }
    }


    // Set up idle notification for delay queue processing if necessary.
    if (SoFlP::delaytimeouttimer) {
        if (sm->isDelaySensorPending()) {
            if (SOFL_DEBUG && 0) { // debug
                SoDebugError::postInfo("SoFlP::sensorQueueChanged",
                    "delaysensor pending");
            }

            if (!SoFlP::delaytimeouttimer->IsRunning()) {
                const SbTime& delaySensorTimeout = SoDB::getDelaySensorTimeout();
                if (delaySensorTimeout != SbTime::zero()) {
                    unsigned long timeout = delaySensorTimeout.getMsecValue();
                    SoFlP::delaytimeouttimer->Start((int)timeout, true);
                }
            }
        }
        else {
            if (SoFlP::delaytimeouttimer->IsRunning())
                SoFlP::delaytimeouttimer->Stop();
        }
    }
}

SoFlP *
SoFlP::instance() {
    static SoFlP singleton;
    return (&singleton);
}

bool
SoFlP::isInitialized() const {
    return (init);
}

void
SoFlP::setInitialize(bool i) {
    init = i;
}

Fl_Window *
SoFlP::getMainFrame() const {
    return (main_frame);
}

void
SoFlP::setMainFrame(Fl_Window * frame) {
    main_frame = frame;
}

#define INIT_TIMER(timer_name, timer_type)  \
    if (!timer_name) {                      \
        timer_name = new timer_type;        \
    }                                       \
    assert(timer_name != 0)

void
SoFlP::initTimers() {
    static bool are_initialized = false;

    if(!are_initialized) {
        INIT_TIMER(SoFlP::timerqueuetimer, TimerQueueTimer);
        INIT_TIMER(SoFlP::delaytimeouttimer, DelayTimeoutTimer);
        are_initialized = true;
    }
}

#undef INIT_TIMER

#define STOP_TIMER(timer_name) if(timer_name) timer_name->Stop()

void
SoFlP::stopTimers() {
    STOP_TIMER(SoFlP::timerqueuetimer);
    wxDELETE(SoFlP::timerqueuetimer);
    STOP_TIMER(SoFlP::delaytimeouttimer);
    wxDELETE(SoFlP::delaytimeouttimer);
}

#undef STOP_TIMER

void
SoFlP::finish() {
#ifdef SOFL_DEBUG
    SoDebugError::postInfo("SoFlP::finish",
                           "remove all internal resources");
#endif
    wxTheApp->Unbind(wxEVT_IDLE, &SoFlP::onIdle, SoFlP::instance());

    stopTimers();

    // only if app is built by SoFl perform exit and cleanup
    if(SoFlP::instance()->is_a_soflp_app) {
        wxTheApp->OnExit();
        wxEntryCleanup();
    }
}

void
SoFlP::onIdle(wxIdleEvent& WXUNUSED(event)) {
#if SOFL_DEBUG && 0
    SoDebugError::postInfo("SoFlP::onIdle",
                               "idlesensor pending");
#endif

    SoDB::getSensorManager()->processTimerQueue();
    SoDB::getSensorManager()->processDelayQueue(true);

    // The change callback is _not_ called automatically from
    // SoSensorManager after the process methods, so we need to
    // explicitly trigger it ourselves here.
    SoGuiP::sensorQueueChanged(NULL);
}

void
SoFlP::onClose(wxCloseEvent& event) {
#if SOFL_DEBUG
    SoDebugError::postInfo("SoFlP::onClose",
                           "clean up in progress!");
#endif

    // turn off timers
    SoFlP::instance()->stopTimers();

    // To avoid getting any further invocations of
    // SoGuiP::sensorQueueChanged() (which would re-allocate the timers
    // we destruct below). This could for instance happen when
    // de-coupling the scenegraph camera, triggering a notification
    // chain through the scenegraph.
    SoDB::getSensorManager()->setChangedCallback(NULL, NULL);

    event.Skip(); // perform destroy
}
