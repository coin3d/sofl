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

#ifndef SOFL_SOFLDIRECTIONALLIGHTEDITOR_H
#define SOFL_SOFLDIRECTIONALLIGHTEDITOR_H

#include <Inventor/SoPath.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/Fl/SoFlComponent.h>

typedef void 	SoFlDirectionalLightEditorCB (void *userData, const SoDirectionalLight *light);

class SoPath;
class SoSeparator;
class SoCamera;
class SoPerspectiveCamera;
class SoNodeSensor;
class SoDirectionalLightManip;
class SoCallbackList;
class SoFlClipboard;
class SoFlRenderArea;
class _SoFlColorEditor;
class _SoFlColorSlider;

// *************************************************************************

typedef void SoFlDirectionalLightEditorCB(
        void * user, const SoDirectionalLight * light);

class SOFL_DLL_API SoFlDirectionalLightEditor : public SoFlComponent {

SOFL_OBJECT_HEADER(SoFlDirectionalLightEditor, SoFlComponent);

public:
    SoFlDirectionalLightEditor(
            Fl_Widget* parent = NULL,
            const char * const name = NULL,
            SbBool embed = TRUE);
    ~SoFlDirectionalLightEditor();

    void attach(SoPath * pathToLight);
    void detach(void);
    SbBool isAttached(void);

    void setLight(const SoDirectionalLight & light);
    const SoDirectionalLight & getLight(void) const;

    void addLightChangedCallback(
            SoFlDirectionalLightEditorCB * callback, void * user = NULL);
    void removeLightChangedCallback(
            SoFlDirectionalLightEditorCB * callback, void * user = NULL);

    virtual void show(void);
    virtual void hide(void);

protected:
    SoFlDirectionalLightEditor(
            Fl_Widget* parent,
            const char * const name,
            SbBool embed,
            SbBool build);

    virtual const char * getDefaultWidgetName(void) const;
    virtual const char * getDefaultTitle(void) const;
    virtual const char * getDefaultIconTitle(void) const;

    SoDirectionalLight * dirLight;
    SoSeparator * root;
    SoSeparator * litStuff;

    SoPerspectiveCamera * myCamera;
    SoCamera * cameraToWatch;

    _SoFlColorEditor * colorEditor;
    _SoFlColorSlider * intensitySlider;
    SoFlRenderArea * renderArea;
    SoNodeSensor * lightSensor;
    SoNodeSensor * cameraSensor;
    SoDirectionalLightManip * dirLightManip;
    static char * geomBuffer;
    SbBool ignoreCallback;
    SoCallbackList * callbackList;

    SoFlClipboard * clipboard;

    void copyLight(SoDirectionalLight * dst, const SoDirectionalLight * src);
    void updateLocalComponents(void);

private:
    Fl_Widget* buildWidget(Fl_Widget* parent);
    void constructor(SbBool build);

}; // class SoFlDirectionalLightEditor

#endif //SOFL_SOFLDIRECTIONALLIGHTEDITOR_H
