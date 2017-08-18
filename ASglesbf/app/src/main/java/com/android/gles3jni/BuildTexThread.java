package com.android.gles3jni;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.egl.EGLSurface;

import com.android.gles3jni.GLESSurfaceView.EGLConfigChooser;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;

public class BuildTexThread extends Thread {
	
	  EGLDisplay mEglDisplay;
	  EGLSurface mEglSurface;
	  EGLConfig mEglConfig;
	  EGLContext mEglContext;
	  EGL10 mEgl;
	  public EGLConfigChooser mEGLConfigChooser;
	  private int EGL_CONTEXT_CLIENT_VERSION = 0x3098; 
	  private Context context;
	    
	public BuildTexThread(Context context){
		this.context = context;
	}
	
	public BuildTexThread(){
	}
	
	public void init(EGLConfig config){
		
		mEgl = (EGL10)EGLContext.getEGL();
		EGL10 egl = EglHelper.mEgl;
		mEglDisplay = mEgl.eglGetDisplay(EGL10.EGL_DEFAULT_DISPLAY);	
		mEglSurface = mEgl.eglGetCurrentSurface(EGL10.EGL_DRAW);
		mEglConfig = EglHelper.mEglConfig;
		int[] attrib_list = {0x3098, 2,EGL10.EGL_NONE };
		mEglContext = mEgl.eglCreateContext(EglHelper.mEglDisplay, mEglConfig, EglHelper.mEglContext, attrib_list); 
		
		mEgl.eglMakeCurrent(EglHelper.mEglDisplay, EglHelper.mEglSurface, EglHelper.mEglSurface, mEglContext);
		Bitmap bmp = Bitmap.createBitmap( 100, 100, Config.ARGB_8888);
		int texId = GLTextureUtils.initImageTexture( bmp, true );
	}
	
	 @Override
	 public void run() {
		 try {
			Thread.sleep(5000);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		init(mEglConfig);
		mEglDisplay = mEgl.eglGetCurrentDisplay();
		mEglSurface = mEgl.eglGetCurrentSurface(EGL10.EGL_DRAW);
		mEgl.eglMakeCurrent(EglHelper.mEglDisplay, EglHelper.mEglSurface, EglHelper.mEglSurface, mEglContext);
		Bitmap bmp = Bitmap.createBitmap( 100, 100, Config.ARGB_8888);
		int texId = GLTextureUtils.initImageTexture( bmp, true );
		//GetTextureId(texId);
	}
	 
	public interface GenTextureId{
		public int GetTextureId(int textId );
	}
	
}
