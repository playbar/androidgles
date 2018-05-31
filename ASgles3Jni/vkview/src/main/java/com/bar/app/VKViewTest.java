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

package com.bar.app;

import android.content.Context;
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
    private static final boolean DEBUG = true;
    
    private final float TOUCH_SCALE_FACOTOR = 180.0f / 320;  
    private Renderer mRender = new Renderer();  

    private float mPreX = 0.0f;  
    private float mPreY = 0.0f;  
    
    public static EGLContext eglContext;
    public static  SurfaceHolder holder;

    public VKViewTest(Context context) {
        super(context);

        setZOrderOnTop(true);
        setEGLConfigChooser(8, 8, 8, 0, 16, 0);
        getHolder().setFormat(PixelFormat.TRANSLUCENT);
        setEGLContextClientVersion(3);
        setRenderer(mRender);
        setRenderMode(VKSurfaceView.RENDERMODE_WHEN_DIRTY);
        holder = getHolder();
    }
    
    @Override  
    public boolean onTouchEvent(MotionEvent event) {  
        // TODO Auto-generated method stub  
    		Log.e("GLES3View", "onTouchEvent");
        float x = event.getX();  
        float y = event.getY();  
          
        switch (event.getAction())   
        {  
            case MotionEvent.ACTION_MOVE:  
            		Log.e("GLES3View", "onTouchEvent->ACTION_MOVE");
                float dx = x - mPreX;  
                float dy = y - mPreY;  
                  
                mRender.zrot = dx * TOUCH_SCALE_FACOTOR;  
                mRender.xrot = dy * TOUCH_SCALE_FACOTOR;  
                  
                this.requestRender();  
                break;  
      
            default:  
                break;  
        }  
        mPreX = x;  
        mPreY = y;  
        return true;  
    }  

    private static class Renderer implements VKSurfaceView.Renderer {
    		
    	  	public float xrot, yrot, zrot;  
          
    	    private static final String TAG = "GLRender";  
    	    private final float[] mProjMatrix   = new float[16];  
    	    private final float[] mVMatrix      = new float[16];  
    	    private final float[] mMVPMatrix    = new float[16];  
    	    private final float[] mRotationMatrix = new float[16];  
    	//  private volatile float mAngle;  
    	      
    	    private CirclePlane mCirclePlane;

        public void onDrawFrame(GL10 gl) {
            Logger.printTime();

            //清楚屏幕和深度缓存  
            GLES30.glClear(GLES30.GL_COLOR_BUFFER_BIT | GLES30.GL_DEPTH_BUFFER_BIT);
              
            Matrix.setLookAtM(mVMatrix, 0, 0, 0, -3, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);  
            Matrix.multiplyMM(mMVPMatrix, 0, mProjMatrix, 0, mVMatrix, 0); 
            
            this.mCirclePlane.Draw(mMVPMatrix);

            return;
        }

        public void onSurfaceChanged(GL10 gl, int width, int height) {
            Log.e("GLES2JNIView", "onSurfaceChanged");
            float ratio = (float) width / height;
            GLES30.glViewport(0, 0, width, height);
            Matrix.frustumM(mProjMatrix, 0, -ratio, ratio, -1, 1, 3, 7);
            return;
        }

        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            Log.e("GLES2JNIView", "onSurfaceCreated");
            //black background
            GLES30.glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            this.mCirclePlane = new CirclePlane(10, 10, 1.0f);
        }

    }  // end of renderer
    
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
    
}
