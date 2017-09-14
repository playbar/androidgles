/*
 * Created by VisualGDB. Based on hello-jni example.
 * Copyright (C) 2009 The Android Open Source Project
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
package com.baofeng.mojing.MojingTextureBaker;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.GLUtils;
import android.view.View;
import android.widget.Button;
import android.os.Bundle;
import android.content.Context;

import com.baofeng.mojing.MojingTextureBaker.MojingTextureRender;

import java.io.InputStream;

public class MojingTextureBaker extends Activity
{
    private GLSurfaceView mGlSurfaceView;
    private Context mContext;
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        mContext = this;
        mGlSurfaceView = new GLSurfaceView(this);
        setContentView(mGlSurfaceView);
        mGlSurfaceView.setRenderer(new MojingTextureRender(this));
        mGlSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
    }

        /* A native method that is implemented by the
         * 'MojingTextureBaker' native library, which is packaged
         * with this application.
         */
        public static native int BeginBaker(int iMaxSize);

        public static native int AddTexture(int hHandle, int iTextureID, int iX, int iY, int iWidth, int iHeight);

        public static native int RemoveTexture(int hHandle, int iTextureID, int iX, int iY, int iWidth, int iHeight);

        public static native int GetBakerTextureSize(int hHandle);

        public static native String BakerTexture(int hHandle, int iTextureID, boolean bReplaceByBakedTexture);

        public static native void EndBaker(int hHandle);

        public static native void OpenGLTest();

		public static native int GLTest(int id);

    /* this is used to load the 'MojingTextureBaker' library on application
     * startup. The library has already been unpacked into
     * /data/data/com.baofeng.mojing.MojingTextureBaker/lib/MojingTextureBaker.so at
     * installation time by the package manager.
     */
        static {
            System.loadLibrary("MojingTextureBaker");
        }
}
