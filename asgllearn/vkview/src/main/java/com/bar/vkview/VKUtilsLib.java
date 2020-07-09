package com.bar.vkview;
import android.content.res.AssetManager;
import android.opengl.GLDebugHelper;
import android.util.Log;
import android.view.Surface;

import java.io.Writer;
import java.lang.ref.WeakReference;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.egl.EGLSurface;
import javax.microedition.khronos.opengles.GL;

/**
 * Created by houguoli on 2018/5/31.
 */

public class VKUtilsLib {

    private static final String VERTEX_SHADER = "shaders/triangle.vert.spv";
    private static final String FRAGMENT_SHADER = "shaders/triangle.frag.spv";
//
//    private static final String VERTEX_SHADER = "shaders/triangle.vert";
//    private static final String FRAGMENT_SHADER = "shaders/triangle.frag";

    private long mNativeVulanUtils;

    ////

    public VKUtilsLib(AssetManager assetManager){
        mNativeVulanUtils = nativeCreate(assetManager, VERTEX_SHADER, FRAGMENT_SHADER);
    }

    public VKUtilsLib(AssetManager assetManager, String vertexShader, String fragmentShader) {
        mNativeVulanUtils = nativeCreate(assetManager, vertexShader, fragmentShader);
    }

    private native long nativeCreate(AssetManager assetManager, String vertexShader,
                                     String fragmentShader);

    private native void nativePause(long nativeHandle);
    private native void nativeResume(long nativeHandle);
    private native void nativeStop(long nativeHandle);
    private native void nativeStart(long nativeHandle);
    private native void nativeCreateSurface(long nativeHandle, Surface surface);
    private native void nativeCleanUp(long nativeHandle);


    private native void nativeOnSurfaceCreated(long nativeHandle);
    private native void nativeOnSurfaceChanged(long nativeHandle);
    private native void nativeOnDrawFrame(long nativeHandle);

    public void start() {
        nativeStart(mNativeVulanUtils);
    }

    public void pause() {
        nativePause(mNativeVulanUtils);
    }

    public void resume() {
        nativeResume(mNativeVulanUtils);
    }


    public void stop() {
        nativeStop(mNativeVulanUtils);
    }


    public void onSurfaceCreated(){
        nativeOnSurfaceCreated(mNativeVulanUtils);
    }

    public void onSurfaceChanged()
    {
        nativeOnSurfaceChanged(mNativeVulanUtils);
    }

    public void onDrawFrame(){
        nativeOnDrawFrame(mNativeVulanUtils);
    }

    //////

    public void setSurfaceView(WeakReference<VKSurfaceView> glSurfaceViewWeakRef) {
        mGLSurfaceViewWeakRef = glSurfaceViewWeakRef;
    }


    /**
     * Create an egl surface for the current SurfaceHolder surface. If a surface
     * already exists, destroy it before creating the new surface.
     *
     * @return true if the surface was created successfully.
     */
    public boolean createSurface() {

        VKSurfaceView view = mGLSurfaceViewWeakRef.get();
        nativeCreateSurface(mNativeVulanUtils, view.getHolder().getSurface());

        return true;
    }


    /**
     * Display the current render surface.
     * @return the EGL error code from eglSwapBuffers.
     */
    public int swap() {
//        if (! mEgl.eglSwapBuffers(mEglDisplay, mEglSurface)) {
//            return mEgl.eglGetError();
//        }
        return EGL10.EGL_SUCCESS;
    }

    public void destroySurface() {
        if (VKSurfaceView.LOG_EGL) {
            Log.w("EglHelper", "destroySurface()  tid=" + Thread.currentThread().getId());
        }
        nativeCleanUp(mNativeVulanUtils);
//        destroySurfaceImp();
    }

    private void destroySurfaceImp() {
//        if (mEglSurface != null && mEglSurface != EGL10.EGL_NO_SURFACE) {
//            mEgl.eglMakeCurrent(mEglDisplay, EGL10.EGL_NO_SURFACE,
//                    EGL10.EGL_NO_SURFACE,
//                    EGL10.EGL_NO_CONTEXT);
//            VKSurfaceView view = mGLSurfaceViewWeakRef.get();
//            if (view != null) {
//                view.mEGLWindowSurfaceFactory.destroySurface(mEgl, mEglDisplay, mEglSurface);
//            }
//            mEglSurface = null;
//        }
    }

    public void finish() {
        if (VKSurfaceView.LOG_EGL) {
            Log.w("EglHelper", "finish() tid=" + Thread.currentThread().getId());
        }
//        if (mEglContext != null) {
//            VKSurfaceView view = mGLSurfaceViewWeakRef.get();
//            if (view != null) {
//                view.mEGLContextFactory.destroyContext(mEgl, mEglDisplay, mEglContext);
//            }
//            mEglContext = null;
//        }
//        if (mEglDisplay != null) {
//            mEgl.eglTerminate(mEglDisplay);
//            mEglDisplay = null;
//        }
    }


    public static void throwEglException(String function, int error) {
        String message = formatEglError(function, error);
        if (VKSurfaceView.LOG_THREADS) {
            Log.e("EglHelper", "throwEglException tid=" + Thread.currentThread().getId() + " "
                    + message);
        }
        throw new RuntimeException(message);
    }

    public static void logEglErrorAsWarning(String tag, String function, int error) {
        Log.w(tag, formatEglError(function, error));
    }

    public static String formatEglError(String function, int error) {
        return function + " failed: ";
    }


    private WeakReference<VKSurfaceView> mGLSurfaceViewWeakRef;



}
