//
// Created by fab on 1/4/26.
//

#ifndef SOFL_CONE_BUILDER_H
#define SOFL_CONE_BUILDER_H

#include <utility>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoSeparator.h>

inline std::pair<SoCamera*,SoSeparator*> buildCameraAndCone()
{
    // Make a scene containing a red cone
    SoSeparator *root = new SoSeparator;
    SoPerspectiveCamera *myCamera = new SoPerspectiveCamera;
    SoMaterial *myMaterial = new SoMaterial;
    root->ref();
    root->addChild(myCamera);
    root->addChild(new SoDirectionalLight);
    myMaterial->diffuseColor.setValue(1.0, 0.0, 0.0);   // Red
    root->addChild(myMaterial);
    root->addChild(new SoCone);
    root->unrefNoDelete();
    return (std::make_pair(myCamera,root));
}
#endif //SOFL_CONE_BUILDER_H