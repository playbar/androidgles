package com.android.gltest;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.egl.EGLSurface;
import javax.microedition.khronos.egl.*;

import android.graphics.Bitmap;
import android.opengl.GLES20;
import android.opengl.GLES30;
import android.opengl.GLUtils;
import android.util.Log;

public class GenTexTask implements Runnable {
	public static EGLContext mEglContext;
	public static EGLDisplay mEglDisplay;
	public static EGLSurface mEglSurface;
	public static EGLConfig mEglConfig;
	private int mHashCode;
	private Bitmap mBmp;
	
    ExportTextureId exprotTex = null;
    
    public void SetInterfaceTex( ExportTextureId tex ){
    		this.exprotTex = tex;
    }
	
	public interface ExportTextureId{
		void exportId( int texid, int hashcode);
	}

	public GenTexTask(int hashcode, Bitmap bmp ) {
		this.mHashCode = hashcode;
		this.mBmp = bmp;
	}

	@Override
	public void run() {
		Log.e("GenTexTask", " begin glMakeCurrent");
		 EGL10 egl = (EGL10)EGLContext.getEGL();
		EGLDisplay eglDisplay = egl.eglGetDisplay(EGL10.EGL_DEFAULT_DISPLAY); // egl.eglGetCurrentDisplay();
		int[] version = new int[2];
		if (!egl.eglInitialize(eglDisplay, version)) {
			throw new RuntimeException("eglInitialize failed");
		}
		
		EGLConfig[] configs = new EGLConfig[1];  
		int[] num_config = new int[1];  
		       //EglchooseConfig used this config  
		int[] configSpec ={  
		    EGL10.EGL_SURFACE_TYPE, EGL10.EGL_PBUFFER_BIT,   
		    EGL10.EGL_RED_SIZE, 8,  
		    EGL10.EGL_GREEN_SIZE, 8,  
		    EGL10.EGL_BLUE_SIZE, 8,  
		    EGL10.EGL_ALPHA_SIZE, 8,  
		    EGL10.EGL_NONE   
		 }; 
//		egl.eglGetConfigs(eglDisplay, configSpec, config_size, num_config);
		egl.eglChooseConfig(eglDisplay, configSpec, configs, 1, num_config);
		
		// EGLSurface readsurface = egl.eglGetCurrentSurface( EGL10.EGL_READ );
		// EGLSurface drawSurface = egl.eglGetCurrentSurface(EGL10.EGL_DRAW);
		// egl.eglMakeCurrent(eglDisplay, drawSurface, readsurface,
		// mEglContext);

//		egl.eglMakeCurrent(eglDisplay, EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_SURFACE, mEglContext);
	       int attribListPbuffer[] = {  
                   EGL10.EGL_WIDTH, 480,  
                   EGL10.EGL_HEIGHT, 800,  
                   EGL10.EGL_NONE  
            };  
        GenTexTask.mEglSurface = egl.eglCreatePbufferSurface( GenTexTask.mEglDisplay, configs[0], attribListPbuffer);
           
		egl.eglMakeCurrent(eglDisplay, EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_SURFACE, mEglContext);
		Log.e("GenTexTask", "glMakeCurrent");
		int textureId = createTexture(mBmp);
		if (null != exprotTex) {
			exprotTex.exportId(textureId, mHashCode);
		}

		return;
	}
	
    public static int createTexture(Bitmap bitmap) { 
        int[] textures = new int[1]; 
        GLES30.glGenTextures(1, textures, 0);
        checkGlError("glGenTexture");
        int textureId = textures[0]; 
        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, textureId); 
        GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MIN_FILTER,GLES30.GL_NEAREST);  
        GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D,GLES30.GL_TEXTURE_MAG_FILTER,GLES30.GL_LINEAR); 
        GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_S,GLES30.GL_CLAMP_TO_EDGE); 
        GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_T,GLES30.GL_CLAMP_TO_EDGE); 
        //上面是纹理贴图的取样方式，包括拉伸方式，取临近值和线性值 
        GLUtils.texImage2D(GLES30.GL_TEXTURE_2D, 0, bitmap, 0);//让图片和纹理关联起来，加载到OpenGl空间中 
        checkGlError("texImage2D");
        Log.d("OPENGL","bitmap:" + bitmap); 
//        bitmap.recycle();//不需要，可以释放 
        return textureId; 
    } 
    
    public static void checkGlError(String op) {
        int error;
        while ((error = GLES20.glGetError()) != GLES20.GL_NO_ERROR) {
            Log.e("GenTexTask", op + ": glError " + error);
            throw new RuntimeException(op + ": glError " + error);
        }
    }
    
}