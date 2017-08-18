package com.android.gles3jni;

import java.lang.ref.WeakReference;
import java.util.ArrayList;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGL11;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.util.Log;

import com.android.util.Logger;

public class GLThread extends Thread {
	
    GLThread(WeakReference<GLESSurfaceView> glSurfaceViewWeakRef) {
        super();
        mWidth = 0;
        mHeight = 0;
        mRequestRender = true;
        mRenderMode = GLESSurfaceView.RENDERMODE_CONTINUOUSLY;
        mGLSurfaceViewWeakRef = glSurfaceViewWeakRef;
    }

    @Override
    public void run() {
        setName("GLThread " + getId());
        if (GLESSurfaceView.LOG_THREADS) {
            Log.i("GLThread", "starting tid=" + getId());
        }

        try {
            guardedRun();
        } catch (InterruptedException e) {
            // fall thru and exit normally
        } finally {
        		GLESSurfaceView.sGLThreadManager.threadExiting(this);
        }
    }

    private void stopEglSurfaceLocked() {
        if (mHaveEglSurface) {
            mHaveEglSurface = false;
            mEglHelper.destroySurface();
        }
    }
    
    public void makeCurrent(){
    		mEglHelper.makeCurrent();
    }
    
    public EGLConfig getEglConfig(){
    		return mEglHelper.getEglConfig();
    }

    private void stopEglContextLocked() {
        if (mHaveEglContext) {
            mEglHelper.finish();
            mHaveEglContext = false;
            GLESSurfaceView.sGLThreadManager.releaseEglContextLocked(this);
        }
    }
    private void guardedRun() throws InterruptedException {
        mEglHelper = new EglHelper(mGLSurfaceViewWeakRef);
        mHaveEglContext = false;
        mHaveEglSurface = false;
        try {
            GL10 gl = null;
            boolean createEglContext = false;
            boolean createEglSurface = false;
            boolean createGlInterface = false;
            boolean lostEglContext = false;
            boolean sizeChanged = false;
            boolean wantRenderNotification = false;
            boolean doRenderNotification = false;
            boolean askedToReleaseEglContext = false;
            int w = 0;
            int h = 0;
            Runnable event = null;

            while (true)
            {
                synchronized (GLESSurfaceView.sGLThreadManager)
                {
                    while (true)
                    {
                        if (mShouldExit)
                        {
                            return;
                        }

                        if (! mEventQueue.isEmpty()) {
                            event = mEventQueue.remove(0);
                            break;
                        }

                        // Update the pause state.
                        boolean pausing = false;
//                        if( mRequestPaused){
//                        		sleep( 10);
//                        		continue;
//                        }
                    
                        if (mPaused != mRequestPaused) {
                            pausing = mRequestPaused;
                            mPaused = mRequestPaused;
                            GLESSurfaceView.sGLThreadManager.notifyAll();
                            if (GLESSurfaceView.LOG_PAUSE_RESUME) {
                                Log.i("GLThread", "mPaused is now " + mPaused + " tid=" + getId());
                            }
                        }

                        // Do we need to give up the EGL context?
                        if (mShouldReleaseEglContext) {
                            if (GLESSurfaceView.LOG_SURFACE) {
                                Log.i("GLThread", "releasing EGL context because asked to tid=" + getId());
                            }
                            stopEglSurfaceLocked();
                            stopEglContextLocked();
                            mShouldReleaseEglContext = false;
                            askedToReleaseEglContext = true;
                        }

                        // Have we lost the EGL context?
                        if (lostEglContext) {
                            stopEglSurfaceLocked();
                            stopEglContextLocked();
                            lostEglContext = false;
                        }

                        // When pausing, release the EGL surface:
                        if (pausing && mHaveEglSurface) {
                            if (GLESSurfaceView.LOG_SURFACE) {
                                Log.i("GLThread", "releasing EGL surface because paused tid=" + getId());
                            }
                            stopEglSurfaceLocked();
                        }

                        // When pausing, optionally release the EGL Context:
                        if (pausing && mHaveEglContext) {
                        	GLESSurfaceView view = mGLSurfaceViewWeakRef.get();
                            boolean preserveEglContextOnPause = view == null ?
                                    false : view.mPreserveEGLContextOnPause;
                            if (!preserveEglContextOnPause || GLESSurfaceView.sGLThreadManager.shouldReleaseEGLContextWhenPausing()) {
                                stopEglContextLocked();
                                if (GLESSurfaceView.LOG_SURFACE) {
                                    Log.i("GLThread", "releasing EGL context because paused tid=" + getId());
                                }
                            }
                        }

                        // When pausing, optionally terminate EGL:
                        if (pausing) {
                            if (GLESSurfaceView.sGLThreadManager.shouldTerminateEGLWhenPausing()) {
                                mEglHelper.finish();
                                if (GLESSurfaceView.LOG_SURFACE) {
                                    Log.i("GLThread", "terminating EGL because paused tid=" + getId());
                                }
                            }
                        }

                        // Have we lost the SurfaceView surface?
                        if ((! mHasSurface) && (! mWaitingForSurface)) {
                            if (GLESSurfaceView.LOG_SURFACE) {
                                Log.i("GLThread", "noticed surfaceView surface lost tid=" + getId());
                            }
                            if (mHaveEglSurface) {
                                stopEglSurfaceLocked();
                            }
                            mWaitingForSurface = true;
                            mSurfaceIsBad = false;
                            GLESSurfaceView.sGLThreadManager.notifyAll();
                        }

                        // Have we acquired the surface view surface?
                        if (mHasSurface && mWaitingForSurface) {
                            if (GLESSurfaceView.LOG_SURFACE) {
                                Log.i("GLThread", "noticed surfaceView surface acquired tid=" + getId());
                            }
                            mWaitingForSurface = false;
                            GLESSurfaceView.sGLThreadManager.notifyAll();
                        }

                        if (doRenderNotification) {
                            if (GLESSurfaceView.LOG_SURFACE) {
                                Log.i("GLThread", "sending render notification tid=" + getId());
                            }
                            wantRenderNotification = false;
                            doRenderNotification = false;
                            mRenderComplete = true;
                            GLESSurfaceView.sGLThreadManager.notifyAll();
                        }

                        // Ready to draw?
                        if (readyToDraw()) {

                            // If we don't have an EGL context, try to acquire one.
                            if (! mHaveEglContext) {
                                if (askedToReleaseEglContext) {
                                    askedToReleaseEglContext = false;
                                } else if (GLESSurfaceView.sGLThreadManager.tryAcquireEglContextLocked(this)) {
                                    try {
                                        mEglHelper.start();
                                    } catch (RuntimeException t) {
                                    	GLESSurfaceView.sGLThreadManager.releaseEglContextLocked(this);
                                        throw t;
                                    }
                                    mHaveEglContext = true;
                                    createEglContext = true;

                                    GLESSurfaceView.sGLThreadManager.notifyAll();
                                }
                            }

                            if (mHaveEglContext && !mHaveEglSurface) {
                                mHaveEglSurface = true;
                                createEglSurface = true;
                                createGlInterface = true;
                                sizeChanged = true;
                            }

                            if (mHaveEglSurface) {
                                if (mSizeChanged) {
                                    sizeChanged = true;
                                    w = mWidth;
                                    h = mHeight;
                                    wantRenderNotification = true;
                                    if (GLESSurfaceView.LOG_SURFACE) {
                                        Log.i("GLThread",
                                                "noticing that we want render notification tid="
                                                + getId());
                                    }

                                    // Destroy and recreate the EGL surface.
                                    createEglSurface = true;

                                    mSizeChanged = false;
                                }
                                mRequestRender = false;
                                GLESSurfaceView.sGLThreadManager.notifyAll();
                                break;
                            }
                        }

                        // By design, this is the only place in a GLThread thread where we wait().
                        if (GLESSurfaceView.LOG_THREADS) {
                            Log.i("GLThread", "waiting tid=" + getId()
                                + " mHaveEglContext: " + mHaveEglContext
                                + " mHaveEglSurface: " + mHaveEglSurface
                                + " mFinishedCreatingEglSurface: " + mFinishedCreatingEglSurface
                                + " mPaused: " + mPaused
                                + " mHasSurface: " + mHasSurface
                                + " mSurfaceIsBad: " + mSurfaceIsBad
                                + " mWaitingForSurface: " + mWaitingForSurface
                                + " mWidth: " + mWidth
                                + " mHeight: " + mHeight
                                + " mRequestRender: " + mRequestRender
                                + " mRenderMode: " + mRenderMode);
                        }
//                        break;
                        GLESSurfaceView.sGLThreadManager.wait();
                    }
                } // end of synchronized(sGLThreadManager)

                if (event != null) {
                    event.run();
                    event = null;
                    continue;
                }

                if (createEglSurface) {
                    if (GLESSurfaceView.LOG_SURFACE) {
                        Log.w("GLThread", "egl createSurface");
                    }
                    if (mEglHelper.createSurface()) {
                        synchronized(GLESSurfaceView.sGLThreadManager) {
                            mFinishedCreatingEglSurface = true;
                            GLESSurfaceView.sGLThreadManager.notifyAll();
                        }
                    } else {
                        synchronized(GLESSurfaceView.sGLThreadManager) {
                            mFinishedCreatingEglSurface = true;
                            mSurfaceIsBad = true;
                            GLESSurfaceView.sGLThreadManager.notifyAll();
                        }
                        continue;
                    }
                    createEglSurface = false;
                }

                if (createGlInterface) {
                    gl = (GL10) mEglHelper.createGL();

                    GLESSurfaceView.sGLThreadManager.checkGLDriver(gl);
                    createGlInterface = false;
                }

                if (createEglContext) {
                    if (GLESSurfaceView.LOG_RENDERER) {
                        Log.w("GLThread", "onSurfaceCreated");
                    }
                    GLESSurfaceView view = mGLSurfaceViewWeakRef.get();
                    if (view != null) {
                        view.mRenderer.onSurfaceCreated(gl, mEglHelper.mEglConfig);
                    }
                    createEglContext = false;
                }

                if (sizeChanged) {
                    if (GLESSurfaceView.LOG_RENDERER) {
                        Log.w("GLThread", "onSurfaceChanged(" + w + ", " + h + ")");
                    }
                    GLESSurfaceView view = mGLSurfaceViewWeakRef.get();
                    if (view != null) {
                        view.mRenderer.onSurfaceChanged(gl, w, h);
                    }
                    sizeChanged = false;
                }

                if (GLESSurfaceView.LOG_RENDERER_DRAW_FRAME) {
                    Log.w("GLThread", "onDrawFrame tid=" + getId());
                }
                {
                	GLESSurfaceView view = mGLSurfaceViewWeakRef.get();
                    if (view != null) {
                        view.mRenderer.onDrawFrame(gl);
                    }
                }
                Logger.printTime();
                int swapError = mEglHelper.swap();
                Logger.printTime();
                switch (swapError) {
                    case EGL10.EGL_SUCCESS:
                        break;
                    case EGL11.EGL_CONTEXT_LOST:
                        if (GLESSurfaceView.LOG_SURFACE) {
                            Log.i("GLThread", "egl context lost tid=" + getId());
                        }
                        lostEglContext = true;
                        break;
                    default:
                        // Other errors typically mean that the current surface is bad,
                        // probably because the SurfaceView surface has been destroyed,
                        // but we haven't been notified yet.
                        // Log the error to help developers understand why rendering stopped.
                        EglHelper.logEglErrorAsWarning("GLThread", "eglSwapBuffers", swapError);

                        synchronized(GLESSurfaceView.sGLThreadManager) {
                            mSurfaceIsBad = true;
                            GLESSurfaceView.sGLThreadManager.notifyAll();
                        }
                        break;
                }

                if (wantRenderNotification) {
                    doRenderNotification = true;
                }
            }

        } finally {
            /*
             * clean-up everything...
             */
            synchronized (GLESSurfaceView.sGLThreadManager) {
                stopEglSurfaceLocked();
                stopEglContextLocked();
            }
        }
    }

    public boolean ableToDraw() {
        return mHaveEglContext && mHaveEglSurface && readyToDraw();
    }

    private boolean readyToDraw() {
        return (!mPaused) && mHasSurface && (!mSurfaceIsBad)
            && (mWidth > 0) && (mHeight > 0)
            && (mRequestRender || (mRenderMode == GLESSurfaceView.RENDERMODE_CONTINUOUSLY));
    }

    public void setRenderMode(int renderMode) {
        if ( !((GLESSurfaceView.RENDERMODE_WHEN_DIRTY <= renderMode) && (renderMode <= GLESSurfaceView.RENDERMODE_CONTINUOUSLY)) ) {
            throw new IllegalArgumentException("renderMode");
        }
        synchronized(GLESSurfaceView.sGLThreadManager) {
            mRenderMode = renderMode;
            GLESSurfaceView.sGLThreadManager.notifyAll();
        }
    }

    public int getRenderMode() {
        synchronized(GLESSurfaceView.sGLThreadManager) {
            return mRenderMode;
        }
    }

    public void requestRender() {
        synchronized(GLESSurfaceView.sGLThreadManager) {
            mRequestRender = true;
            GLESSurfaceView.sGLThreadManager.notifyAll();
        }
    }

    public void surfaceCreated() {
        synchronized(GLESSurfaceView.sGLThreadManager) {
            if (GLESSurfaceView.LOG_THREADS) {
                Log.i("GLThread", "surfaceCreated tid=" + getId());
            }
            mHasSurface = true;
            mFinishedCreatingEglSurface = false;
            GLESSurfaceView.sGLThreadManager.notifyAll();
            while (mWaitingForSurface
                   && !mFinishedCreatingEglSurface
                   && !mExited) {
                try {
                		GLESSurfaceView.sGLThreadManager.wait();
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            }
        }
    }

    public void surfaceDestroyed() {
        synchronized(GLESSurfaceView.sGLThreadManager) {
            if (GLESSurfaceView.LOG_THREADS) {
                Log.i("GLThread", "surfaceDestroyed tid=" + getId());
            }
            mHasSurface = false;
            GLESSurfaceView.sGLThreadManager.notifyAll();
            while((!mWaitingForSurface) && (!mExited)) {
                try {
                	GLESSurfaceView.sGLThreadManager.wait();
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            }
        }
    }

    public void onPause() {
        synchronized (GLESSurfaceView.sGLThreadManager) {
            if (GLESSurfaceView.LOG_PAUSE_RESUME) {
                Log.i("GLThread", "onPause tid=" + getId());
            }
            mRequestPaused = true;
            GLESSurfaceView.sGLThreadManager.notifyAll();
            while ((! mExited) && (! mPaused)) {
                if (GLESSurfaceView.LOG_PAUSE_RESUME) {
                    Log.i("Main thread", "onPause waiting for mPaused.");
                }
                try {
                	GLESSurfaceView.sGLThreadManager.wait();
                } catch (InterruptedException ex) {
                    Thread.currentThread().interrupt();
                }
            }
        }
    }

    public void onResume() {
        synchronized (GLESSurfaceView.sGLThreadManager) {
            if (GLESSurfaceView.LOG_PAUSE_RESUME) {
                Log.i("GLThread", "onResume tid=" + getId());
            }
            mRequestPaused = false;
            mRequestRender = true;
            mRenderComplete = false;
            GLESSurfaceView.sGLThreadManager.notifyAll();
            while ((! mExited) && mPaused && (!mRenderComplete)) {
                if (GLESSurfaceView.LOG_PAUSE_RESUME) {
                    Log.i("Main thread", "onResume waiting for !mPaused.");
                }
                try {
                	GLESSurfaceView.sGLThreadManager.wait();
                } catch (InterruptedException ex) {
                    Thread.currentThread().interrupt();
                }
            }
        }
    }

    public void onWindowResize(int w, int h) {
        synchronized (GLESSurfaceView.sGLThreadManager) {
            mWidth = w;
            mHeight = h;
            mSizeChanged = true;
            mRequestRender = true;
            mRenderComplete = false;
            GLESSurfaceView.sGLThreadManager.notifyAll();

            // Wait for thread to react to resize and render a frame
            while (! mExited && !mPaused && !mRenderComplete
                    && ableToDraw()) {
                if (GLESSurfaceView.LOG_SURFACE) {
                    Log.i("Main thread", "onWindowResize waiting for render complete from tid=" + getId());
                }
                try {
                	GLESSurfaceView.sGLThreadManager.wait();
                } catch (InterruptedException ex) {
                    Thread.currentThread().interrupt();
                }
            }
        }
    }

    public void requestExitAndWait() {
        // don't call this from GLThread thread or it is a guaranteed
        // deadlock!
        synchronized(GLESSurfaceView.sGLThreadManager) {
            mShouldExit = true;
            GLESSurfaceView.sGLThreadManager.notifyAll();
            while (! mExited) {
                try {
                	GLESSurfaceView.sGLThreadManager.wait();
                } catch (InterruptedException ex) {
                    Thread.currentThread().interrupt();
                }
            }
        }
    }

    public void requestReleaseEglContextLocked() {
        mShouldReleaseEglContext = true;
        GLESSurfaceView.sGLThreadManager.notifyAll();
    }

    /**
     * Queue an "event" to be run on the GL rendering thread.
     * @param r the runnable to be run on the GL rendering thread.
     */
    public void queueEvent(Runnable r) {
        if (r == null) {
            throw new IllegalArgumentException("r must not be null");
        }
        synchronized(GLESSurfaceView.sGLThreadManager) {
            mEventQueue.add(r);
            GLESSurfaceView.sGLThreadManager.notifyAll();
        }
    }

    // Once the thread is started, all accesses to the following member
    // variables are protected by the sGLThreadManager monitor
    private boolean mShouldExit;
    public boolean mExited;
    private boolean mRequestPaused;
    private boolean mPaused;
    private boolean mHasSurface;
    private boolean mSurfaceIsBad;
    private boolean mWaitingForSurface;
    private boolean mHaveEglContext;
    private boolean mHaveEglSurface;
    private boolean mFinishedCreatingEglSurface;
    private boolean mShouldReleaseEglContext;
    private int mWidth;
    private int mHeight;
    private int mRenderMode;
    private boolean mRequestRender;
    private boolean mRenderComplete;
    private ArrayList<Runnable> mEventQueue = new ArrayList<Runnable>();
    private boolean mSizeChanged = true;

    // End of member variables protected by the sGLThreadManager monitor.

    private EglHelper mEglHelper;

    /**
     * Set once at thread construction time, nulled out when the parent view is garbage
     * called. This weak reference allows the GLSurfaceView to be garbage collected while
     * the GLThread is still alive.
     */
    private WeakReference<GLESSurfaceView> mGLSurfaceViewWeakRef;

}

