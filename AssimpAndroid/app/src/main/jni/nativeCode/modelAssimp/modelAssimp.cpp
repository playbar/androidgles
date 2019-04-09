/*
 *    Copyright 2016 Anand Muralidhar
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include "myShader.h"
#include "modelAssimp.h"
#include <glm/gtc/type_ptr.hpp>

#include "assimp/Importer.hpp"
#include "Camera.h"
#include <opencv2/opencv.hpp>
#include <myJNIHelper.h>

/**
 * Class constructor
 */
ModelAssimp::ModelAssimp() {

    MyLOGD("ModelAssimp::ModelAssimp");
    initsDone = false;

    // create MyGLCamera object and set default position for the object
    myGLCamera = new MyGLCamera();
    float pos[]={0.,0.,0.,0.2,0.5,0.};
    std::copy(&pos[0], &pos[5], std::back_inserter(modelDefaultPosition));
    myGLCamera->SetModelPosition(modelDefaultPosition);

    std::string vertexShader    = "shaders/model_loading.vs";
    std::string fragmentShader  = "shaders/model_loading.frag";
    mshader.Init(vertexShader.c_str(), fragmentShader.c_str());

//    modelObject = NULL;
}

ModelAssimp::~ModelAssimp() {

    MyLOGD("ModelAssimp::ModelAssimpssimp");
    if (myGLCamera) {
        delete myGLCamera;
    }
//    if (modelObject) {
//        delete modelObject;
//    }
}

/**
 * Perform inits and load the triangle's vertices/colors to GLES
 */
void ModelAssimp::PerformGLInits() {

    MyLOGD("ModelAssimp::PerformGLInits");

    MyGLInits();

//    modelObject = new AssimpLoader();
    mModel = new Model();

    // extract the OBJ and companion files from assets
    std::string objFilename, mtlFilename, texFilename, str11,str22, strBG2, strlattice, strloftstep;
    std::string strTmp;
//    bool isFilesPresent  =
//            gHelperObject->ExtractAssetReturnFilename("amenemhat/amenemhat.obj", objFilename) &&
//            gHelperObject->ExtractAssetReturnFilename("amenemhat/amenemhat.mtl", mtlFilename) &&
//            gHelperObject->ExtractAssetReturnFilename("amenemhat/amenemhat.jpg", texFilename);
//    bool isFilesPresent  =
//            gHelperObject->ExtractAssetReturnFilename("space/space_screen03.FBX", objFilename) &&
//            gHelperObject->ExtractAssetReturnFilename("space/11.jpg", str11) &&
//            gHelperObject->ExtractAssetReturnFilename("space/22.jpg", str22) &&
//            gHelperObject->ExtractAssetReturnFilename("space/BG2.jpg", strBG2 ) &&
//            gHelperObject->ExtractAssetReturnFilename("space/lattice.jpg", strlattice ) &&
//            gHelperObject->ExtractAssetReturnFilename("space/loftstep.jpg", strloftstep ) &&
//            gHelperObject->ExtractAssetReturnFilename("space/mainspace.jpg", strTmp ) &&
//            gHelperObject->ExtractAssetReturnFilename("space/pingmu.jpg", strTmp ) &&
//            gHelperObject->ExtractAssetReturnFilename("space/roomspace.jpg", strTmp ) &&
//            gHelperObject->ExtractAssetReturnFilename("space/screen_var.jpg", strTmp ) &&
//            gHelperObject->ExtractAssetReturnFilename("space/seat_.jpg", strTmp ) &&
//            gHelperObject->ExtractAssetReturnFilename("space/space_.jpg", strTmp ) &&
//            gHelperObject->ExtractAssetReturnFilename("space/textures.2.png", strTmp ) &&
//            gHelperObject->ExtractAssetReturnFilename("space/TV1.jpg", strTmp ) &&
//            gHelperObject->ExtractAssetReturnFilename("space/TV2.jpg", strTmp ) &&
//            gHelperObject->ExtractAssetReturnFilename("space/TV3.jpg", strTmp ) &&
//            gHelperObject->ExtractAssetReturnFilename("space/TV4.jpg", strTmp ) &&
//            gHelperObject->ExtractAssetReturnFilename("space/TV5.jpg", strTmp ) &&
//            gHelperObject->ExtractAssetReturnFilename("space/TV6.jpg", strTmp );


//    bool isFilesPresent  = gHelperObject->ExtractAssetReturnFilename("scene/door.FBX", objFilename);
    bool isFilesPresent  = gHelperObject->ExtractAssetReturnFilename("spider/spider.obj", objFilename)
                           && gHelperObject->ExtractAssetReturnFilename("spider/spider.mtl", objFilename)
                           && gHelperObject->ExtractAssetReturnFilename("spider/drkwood2.jpg", strTmp )
                           && gHelperObject->ExtractAssetReturnFilename("spider/engineflare1.jpg", strTmp )
                           && gHelperObject->ExtractAssetReturnFilename("spider/SpiderTex.jpg", strTmp )
                           && gHelperObject->ExtractAssetReturnFilename("spider/wal67ar_small.jpg", strTmp );
//
//    bool isFilesPresent  = gHelperObject->ExtractAssetReturnFilename("astroboy/astroboy_walk.dae", objFilename)
//                           &&gHelperObject->ExtractAssetReturnFilename("astroboy/seymour.jpg", objFilename);

    if( !isFilesPresent ) {
        MyLOGE("Model %s does not exist!", objFilename.c_str());
        return;
    }

//    modelObject->Load3DModel(objFilename);
    mModel->loadModel(objFilename);

    CheckGLError("ModelAssimp::PerformGLInits");
    initsDone = true;
}


/**
 * Render to the display
 */
void ModelAssimp::Render() {

    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    glm::mat4 mvpMat = myGLCamera->GetMVP();
//    modelObject->Render3DModel(&mvpMat);

    // Clear the colorbuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.5f, 0.05f, 0.05f, 1.0f);
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
    mshader.Use();
    // Transformation matrices
    glm::mat4 projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glUniformMatrix4fv(glGetUniformLocation(mshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(mshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

    // Draw the loaded model
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
    glUniformMatrix4fv(glGetUniformLocation(mshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    mModel->Draw(mshader);

    CheckGLError("ModelAssimp::Render");

}

/**
 * set the viewport, function is also called when user changes device orientation
 */
void ModelAssimp::SetViewport(int width, int height) {

    screenHeight = height;
    screenWidth = width;
    glViewport(0, 0, width, height);
    CheckGLError("Cube::SetViewport");

    myGLCamera->SetAspectRatio((float) width / height);
}


/**
 * reset model's position in double-tap
 */
void ModelAssimp::DoubleTapAction() {

    myGLCamera->SetModelPosition(modelDefaultPosition);
}

/**
 * rotate the model if user scrolls with one finger
 */
void ModelAssimp::ScrollAction(float distanceX, float distanceY, float positionX, float positionY) {

    myGLCamera->RotateModel(distanceX, distanceY, positionX, positionY);
}

/**
 * pinch-zoom: move the model closer or farther away
 */
void ModelAssimp::ScaleAction(float scaleFactor) {

    myGLCamera->ScaleModel(scaleFactor);
}

/**
 * two-finger drag: displace the model by changing its x-y coordinates
 */
void ModelAssimp::MoveAction(float distanceX, float distanceY) {

    myGLCamera->TranslateModel(distanceX, distanceY);
}