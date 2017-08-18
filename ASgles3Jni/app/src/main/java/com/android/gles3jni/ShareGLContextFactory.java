package com.android.gles3jni;

import javax.microedition.khronos.egl.EGL10;

import com.android.gles3jni.GLESSurfaceView.EGLContextFactory;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGL11;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.egl.EGLSurface;
import javax.microedition.khronos.opengles.GL;
import javax.microedition.khronos.opengles.GL10;

import android.util.Log;

class ShareGLContextFactory implements EGLContextFactory {  
    private int EGL_CONTEXT_CLIENT_VERSION = 0x3098;  
    private EGLContext mShareContext = null;  
      
    ShareGLContextFactory(EGLContext shareContext) {  
        mShareContext = shareContext;  
    }  
  
    @Override
    public EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig config)
    {  
        int[] attrib_list = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL10.EGL_NONE };  
        return egl.eglCreateContext(display, config, mShareContext, attrib_list);  
    }  
  
    public void destroyContext(EGL10 egl, EGLDisplay display,  EGLContext context) {  
        if (!egl.eglDestroyContext(display, context)) {  
            Log.e("DefaultContextFactory", "display:" + display + " context: " + context);  
        }  
    }
 
}  