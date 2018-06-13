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

package com.bar.vkview;

import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.PixelFormat;
import android.opengl.GLES30;
import android.opengl.Matrix;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.opengles.GL10;

import com.bar.util.Logger;

public class VKViewTest extends VKSurfaceView {

    private static final String TAG = "VKView";


    private VKViewRenderer mRender = new VKViewRenderer();

//    VKUtilsLib mVKUitlsLib;

    private float mPreX = 0.0f;
    private float mPreY = 0.0f;

    public static EGLContext eglContext;
    public static  SurfaceHolder holder;
    private AssetManager mAssetManager;

    public VKViewTest(Context context) {
        super(context);

        mAssetManager = getContext().getAssets();

//        mVKUitlsLib = new VKUtilsLib(mAssetManager, VERTEX_SHADER, FRAGMENT_SHADER);

        setZOrderOnTop(true);
        setEGLConfigChooser(8, 8, 8, 0, 16, 0);
        getHolder().setFormat(PixelFormat.TRANSLUCENT);
        setEGLContextClientVersion(3);
        setRenderer(mRender);
//        setRenderMode(VKSurfaceView.RENDERMODE_WHEN_DIRTY);
        holder = getHolder();
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        // TODO Auto-generated method stub
        Log.e("GLES3View", "onTouchEvent");
        float x = event.getX();
        float y = event.getY();

        return true;
    }


    public static int loadShader(int type, String shaderCode)
    {
        int shader = GLES30.glCreateShader(type);
        GLES30.glShaderSource(shader, shaderCode);
        GLES30.glCompileShader(shader);

        return shader;
    }

    public static void checkGLError(String glOperation)
    {
        int error;
        while((error = GLES30.glGetError()) != (GLES30.GL_NO_ERROR))
        {
            Log.e(TAG, glOperation + ": glError " + error);
            throw new RuntimeException(glOperation + ": glError " + error);
        }
    }

    /////
    public class VKViewRenderer implements VKSurfaceView.Renderer{

        private static final String TAG = "GLRender";
        private final float[] mProjMatrix   = new float[16];
        private final float[] mVMatrix      = new float[16];
        private final float[] mMVPMatrix    = new float[16];
        private final float[] mRotationMatrix = new float[16];
        //  private volatile float mAngle;

        public void onDrawFrame() {
            Logger.printTime();
            VKViewTest.this.mVKUtilsLib.onDrawFrame();

            return;
        }

        public void onSurfaceChanged(int width, int height) {
            Log.e("GLES2JNIView", "onSurfaceChanged");
            VKViewTest.this.mVKUtilsLib.onSurfaceChanged();
            return;
        }

        public void onSurfaceCreated(EGLConfig config) {
            Log.e("GLES2JNIView", "onSurfaceCreated");
            VKViewTest.this.mVKUtilsLib.onSurfaceCreated();

        }

    }


}
