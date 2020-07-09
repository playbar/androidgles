/*
 * Copyright 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.bar.glview;

import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class GLES3JNIView extends GLSurfaceView {
    private static final String TAG = "GLES3JNI";
    private static final boolean DEBUG = true;
    private GLES3JNILib mJniLib;

    public GLES3JNIView(Context context) {
        super(context);
        mJniLib = new GLES3JNILib();

        // Pick an EGLConfig with RGB8 color, 16-bit depth, no stencil,
        // supporting OpenGL ES 2.0 or later backwards-compatible versions.
        setZOrderOnTop(true);
        setEGLConfigChooser(8, 8, 8, 0, 16, 0);
        getHolder().setFormat(PixelFormat.TRANSLUCENT);
        setEGLContextClientVersion(3);
        setRenderer(new Renderer());
//        start();
    }

    public void surfaceCreated(final SurfaceHolder holder)
    {
       super.surfaceCreated(holder);
        AssetManager assetmgr = getContext().getResources().getAssets();
        SurfaceHolder holder1 = getHolder();
        Surface surface = holder1.getSurface();
//        mJniLib.run(surface, assetmgr);
//        mJniLib.initVK(holder.getSurface(), assetmgr);
//        mJniLib.run(holder.getSurface(), assetmgr);
        Log.e("test", "create vulkan success");
        return;
    }

    private class Renderer implements GLSurfaceView.Renderer {
        public void onDrawFrame(GL10 gl) {
            Logger.printFPS();
//            Logger.printTime("begin->");
//            GLES3JNILib.vkDrawFrame();
//            mJniLib.step();
//            Logger.printTime("end->");
        }

        public void onSurfaceChanged(GL10 gl, int width, int height) {
            Logger.printTime();
//            mJniLib.resize(width, height);
        }

        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            Logger.printTime();
//            GLES3Hook.hookTest();

//            mJniLib.init();
        }
    }
}
