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

package com.android.gles3jni;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.PixelFormat;
import android.graphics.Bitmap.Config;
import android.opengl.GLES30;
import android.opengl.GLSurfaceView;
import android.opengl.Matrix;
import android.util.AttributeSet;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

import java.nio.FloatBuffer;
import java.util.Timer;
import java.util.TimerTask;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.egl.EGLSurface;
import javax.microedition.khronos.opengles.GL10;

import com.android.gltest.MyDrawModel;

class GLES3View extends GLESSurfaceView {
    private static final String TAG = "GLES3JNI";
    private static final boolean DEBUG = true;
    
    private final float TOUCH_SCALE_FACOTOR = 180.0f / 320;  
    private Renderer mRender = new Renderer();  
    MyDrawModel drawModel;
      
    private float mPreX = 0.0f;  
    private float mPreY = 0.0f;  
    
    public static EGLContext eglContext;
    public static  SurfaceHolder holder;

    public GLES3View(Context context) {
        super(context);
        // Pick an EGLConfig with RGB8 color, 16-bit depth, no stencil,
        // supporting OpenGL ES 2.0 or later backwards-compatible versions.
        setZOrderOnTop(true);
        setEGLConfigChooser(8, 8, 8, 0, 16, 0);
        getHolder().setFormat(PixelFormat.TRANSLUCENT);
        setEGLContextClientVersion(3);
        setRenderer(mRender);
        start();
        setRenderMode(GLESSurfaceView.RENDERMODE_WHEN_DIRTY);  
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
    
    public final static TimerTask task = new TimerTask(){   
    	
        public void run(){   
            EGL10 egl = (EGL10)EGLContext.getEGL();
            EGLDisplay eglDisplay =egl.eglGetDisplay(EGL10.EGL_DEFAULT_DISPLAY); // egl.eglGetCurrentDisplay();
//      	  	EGLSurface eglSurface = egl.eglGetCurrentSurface(EGL10.EGL_DRAW);
            
//            EGLDisplay eglDisplay = EglHelper.mEglDisplay;
//      	  	EGLSurface eglSurface = EGL10.EGL_NO_SURFACE; // EglHelper.mEglSurface;
//      	  	EGLSurface eglSurface = egl.eglCreateWindowSurface(eglDisplay, EglHelper.mEglConfig, holder, null);
//      	  	int error = egl.eglGetError();
//      	  	eglContext = egl.eglGetCurrentContext();
//      	  	int[] attrib_list = {0x3098, 2,EGL10.EGL_NONE };
//      	  	eglContext = egl.eglCreateContext(eglDisplay, EglHelper.mEglConfig, EglHelper.mEglContext, attrib_list); 
//          eglContext = egl.eglCreateContext(EglHelper.mEglDisplay, EglHelper.mEglConfig, EglHelper.mEglContext, attrib_list); 
      	  	egl.eglMakeCurrent(eglDisplay, EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_SURFACE, eglContext);
//      	  	error = egl.eglGetError();
      		Bitmap bmp = Bitmap.createBitmap( 100, 100, Config.ARGB_8888);
      		int texId = GLTextureUtils.initImageTexture( bmp, true );
      		return;
        }   
    };   

    private static class Renderer implements GLESSurfaceView.Renderer {
    		
    	  	public float xrot, yrot, zrot;  
          
    	    private static final String TAG = "GLRender";  
    	    private final float[] mProjMatrix   = new float[16];  
    	    private final float[] mVMatrix      = new float[16];  
    	    private final float[] mMVPMatrix    = new float[16];  
    	    private final float[] mRotationMatrix = new float[16];  
    	//  private volatile float mAngle;  
    	      
    	    private CirclePlane mCirclePlane;  
    	    //定义环境光  
    	    private FloatBuffer lightAmbient    = FloatBuffer.wrap(new float[]{0.5f, 0.5f, 0.5f, 1.0f});  
    	    //定义漫散射  
    	    private FloatBuffer lightDiffuse    = FloatBuffer.wrap(new float[]{1.0f, 1.0f, 1.0f, 1.0f});  
    	    //光源的位置  
    	    private FloatBuffer lightPosition   = FloatBuffer.wrap(new float[]{0.0f, 0.0f, 2.0f, 1.0f}); 
    	    
    	    public Renderer(){
    	    	
    	    }
    	    
        public void onDrawFrame(GL10 gl) {
//        		Log.e("GLES2JNIView", "onDrawFrame");
            // TODO Auto-generated method stub  
            //清楚屏幕和深度缓存  
            GLES30.glClear(GLES30.GL_COLOR_BUFFER_BIT | GLES30.GL_DEPTH_BUFFER_BIT);  
              
            Matrix.setLookAtM(mVMatrix, 0, 0, 0, -3, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);  
            Matrix.multiplyMM(mMVPMatrix, 0, mProjMatrix, 0, mVMatrix, 0); 
            
            this.mCirclePlane.Draw(mMVPMatrix);
            return;
        }

        public void TestMakeCurrent(){
      	  	Timer timer = new Timer(); 
      	  	timer.schedule(task, 5000 ); 
        }
        
        public void onSurfaceChanged(GL10 gl, int width, int height) {
        		Log.e("GLES2JNIView", "onSurfaceChanged");
        	      float ratio = (float) width / height;  
        	        //设置OPENGL视口  
        	        GLES30.glViewport(0, 0, width, height);  
        	        //设置矩阵投影参数  
        	        Matrix.frustumM(mProjMatrix, 0, -ratio, ratio, -1, 1, 3, 7); 

        	        return;
        }

        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        		Log.e("GLES2JNIView", "onSurfaceCreated");
        		//black background  
            GLES30.glClearColor(0.0f, 0.0f, 0.0f, 0.0f);  
            this.mCirclePlane = new CirclePlane(10, 20, 1.0f);  
            
//          EGL10 egl = (EGL10)EGLContext.getEGL();
//          EGLDisplay eglDisplay = egl.eglGetCurrentDisplay();
//    	  	EGLSurface eglSurface = egl.eglGetCurrentSurface(EGL10.EGL_DRAW);
////    	  	EGLConfig eglConfig 
//    	  	EGLContext eglContext = egl.eglGetCurrentContext();
//    	  	int[] attrib_list = {0x3098, 2,EGL10.EGL_NONE };
//    	  	eglContext = egl.eglCreateContext(EglHelper.mEglDisplay, EglHelper.mEglConfig, EglHelper.mEglContext, attrib_list); 
//		
//    	  	egl.eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
//		
	        int[] attrib_list = {0x3098, 3,EGL10.EGL_NONE };
	        EGL10 egl = (EGL10)EGLContext.getEGL();
	        eglContext = egl.eglCreateContext(EglHelper.mEglDisplay, EglHelper.mEglConfig, EglHelper.mEglContext, attrib_list); 
      	  	
            Bitmap bmp = Bitmap.createBitmap( 100, 100, Config.ARGB_8888);
    			int texId = GLTextureUtils.initImageTexture( bmp, true );
    			TestMakeCurrent();
        }
       
    }  // end of renderer
    
    public static int loadShader(int type, String shaderCode)  
    {  
        // create a shader type (GLES30.GL_VERTEX_SHADER)  
        // or a fragment shader type (GLES30.GL_FRAGMENT_SHADER)  
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
