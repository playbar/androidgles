package com.bar.vkview;

import java.lang.ref.WeakReference;
import java.util.ArrayList;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGL11;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.util.Log;
import android.view.SurfaceHolder;

import com.bar.util.Logger;

public class VKThread extends Thread {
	
    VKThread(WeakReference<VKSurfaceView> glSurfaceViewWeakRef) {
        super();
        mWidth = 0;
        mHeight = 0;
        mRequestRender = true;
        mRenderMode = VKSurfaceView.RENDERMODE_CONTINUOUSLY;
        mGLSurfaceViewWeakRef = glSurfaceViewWeakRef;
    }

    @Override
    public void run() {
        setName("VKThread " + getId());
        if (VKSurfaceView.LOG_THREADS) {
            Log.i("VKThread", "starting tid=" + getId());
        }
        VKSurfaceView view = mGLSurfaceViewWeakRef.get();
        SurfaceHolder holder = view.getHolder();
//        view.mVKUtilsLib.run(holder.getSurface());
        try {
            guardedRun();
        } catch (InterruptedException e) {
            // fall thru and exit normally
        } finally {
            VKSurfaceView.sGLThreadManager.threadExiting(this);
        }
    }

    private void stopEglSurfaceLocked() {
        if (mHaveEglSurface) {
            mHaveEglSurface = false;
            mWidth = 0;
            mHeight = 0;
            VKSurfaceView view = mGLSurfaceViewWeakRef.get();
            view.mVKUtilsLib.destroySurface();
//            mEglHelper.destroySurface();
        }
    }
    

    private void stopEglContextLocked() {
        if (mHaveEglContext) {
            VKSurfaceView view = mGLSurfaceViewWeakRef.get();
            view.mVKUtilsLib.finish();
//            mEglHelper.finish();
            mHaveEglContext = false;
            VKSurfaceView.sGLThreadManager.releaseEglContextLocked(this);
        }
    }
    private void guardedRun() throws InterruptedException {
        VKSurfaceView view11 = mGLSurfaceViewWeakRef.get();
        view11.mVKUtilsLib.setSurfaceView(mGLSurfaceViewWeakRef);
//        mEglHelper = new EglHelper(mGLSurfaceViewWeakRef);
        mHaveEglContext = false;
        mHaveEglSurface = false;
        try {
//            GL10 gl = null;
            boolean createEglContext = false;
            boolean createEglSurface = false;
//            boolean createGlInterface = false;
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
                synchronized (VKSurfaceView.sGLThreadManager)
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

                        if (mPaused != mRequestPaused) {
                            pausing = mRequestPaused;
                            mPaused = mRequestPaused;
                            VKSurfaceView.sGLThreadManager.notifyAll();
                            if (VKSurfaceView.LOG_PAUSE_RESUME) {
                                Log.i("VKThread", "mPaused is now " + mPaused + " tid=" + getId());
                            }
                        }

                        // Do we need to give up the EGL context?
                        if (mShouldReleaseEglContext) {
                            if (VKSurfaceView.LOG_SURFACE) {
                                Log.i("VKThread", "releasing EGL context because asked to tid=" + getId());
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
                            if (VKSurfaceView.LOG_SURFACE) {
                                Log.i("VKThread", "releasing EGL surface because paused tid=" + getId());
                            }
                            stopEglSurfaceLocked();
                        }

                        // When pausing, optionally release the EGL Context:
                        if (pausing && mHaveEglContext) {
                        	VKSurfaceView view = mGLSurfaceViewWeakRef.get();
                            boolean preserveEglContextOnPause = view == null ?
                                    false : view.mPreserveEGLContextOnPause;
                            if (!preserveEglContextOnPause || VKSurfaceView.sGLThreadManager.shouldReleaseEGLContextWhenPausing()) {
                                stopEglContextLocked();
                                if (VKSurfaceView.LOG_SURFACE) {
                                    Log.i("VKThread", "releasing EGL context because paused tid=" + getId());
                                }
                            }
                        }

                        // When pausing, optionally terminate EGL:
                        if (pausing) {
                            if (VKSurfaceView.sGLThreadManager.shouldTerminateEGLWhenPausing()) {
                                VKSurfaceView view = mGLSurfaceViewWeakRef.get();
                                view.mVKUtilsLib.finish();
//                                mEglHelper.finish();
                                if (VKSurfaceView.LOG_SURFACE) {
                                    Log.i("VKThread", "terminating EGL because paused tid=" + getId());
                                }
                            }
                        }

                        // Have we lost the SurfaceView surface?
                        if ((! mHasSurface) && (! mWaitingForSurface)) {
                            if (VKSurfaceView.LOG_SURFACE) {
                                Log.i("VKThread", "noticed surfaceView surface lost tid=" + getId());
                            }
                            if (mHaveEglSurface) {
                                stopEglSurfaceLocked();
                            }
                            mWaitingForSurface = true;
                            mSurfaceIsBad = false;
                            VKSurfaceView.sGLThreadManager.notifyAll();
                        }

                        // Have we acquired the surface view surface?
                        if (mHasSurface && mWaitingForSurface) {
                            if (VKSurfaceView.LOG_SURFACE) {
                                Log.i("VKThread", "noticed surfaceView surface acquired tid=" + getId());
                            }
                            mWaitingForSurface = false;
                            VKSurfaceView.sGLThreadManager.notifyAll();
                        }

                        if (doRenderNotification) {
                            if (VKSurfaceView.LOG_SURFACE) {
                                Log.i("VKThread", "sending render notification tid=" + getId());
                            }
                            wantRenderNotification = false;
                            doRenderNotification = false;
                            mRenderComplete = true;
                            VKSurfaceView.sGLThreadManager.notifyAll();
                        }

                        // Ready to draw?
                        if (readyToDraw()) {

                            // If we don't have an EGL context, try to acquire one.
                            if (! mHaveEglContext) {
                                if (askedToReleaseEglContext) {
                                    askedToReleaseEglContext = false;
                                } else if (VKSurfaceView.sGLThreadManager.tryAcquireEglContextLocked(this)) {
                                    try {
                                        VKSurfaceView view = mGLSurfaceViewWeakRef.get();
                                        view.mVKUtilsLib.start();
//                                        mEglHelper.start();
                                    } catch (RuntimeException t) {
                                    	VKSurfaceView.sGLThreadManager.releaseEglContextLocked(this);
                                        throw t;
                                    }
                                    mHaveEglContext = true;
                                    createEglContext = true;

                                    VKSurfaceView.sGLThreadManager.notifyAll();
                                }
                            }

                            if (mHaveEglContext && !mHaveEglSurface) {
                                mHaveEglSurface = true;
                                createEglSurface = true;
//                                createGlInterface = true;
                                sizeChanged = true;
                            }

                            if (mHaveEglSurface) {
                                if (mSizeChanged) {
                                    sizeChanged = true;
                                    w = mWidth;
                                    h = mHeight;
                                    wantRenderNotification = true;
                                    if (VKSurfaceView.LOG_SURFACE) {
                                        Log.i("VKThread",
                                                "noticing that we want render notification tid="
                                                + getId());
                                    }

                                    // Destroy and recreate the EGL surface.
                                    createEglSurface = true;

                                    mSizeChanged = false;
                                }
                                mRequestRender = false;
                                VKSurfaceView.sGLThreadManager.notifyAll();
                                break;
                            }
                        }

//                        break;
                        VKSurfaceView.sGLThreadManager.wait();
                    }
                } // end of synchronized(sGLThreadManager)

                if (event != null) {
                    event.run();
                    event = null;
                    continue;
                }

                if (createEglSurface) {
                    if (VKSurfaceView.LOG_SURFACE) {
                        Log.w("VKThread", "egl createSurface");
                    }
//                    if (mEglHelper.createSurface())
                    VKSurfaceView view = mGLSurfaceViewWeakRef.get();
                    if(view.mVKUtilsLib.createSurface())
                    {
                        synchronized(VKSurfaceView.sGLThreadManager) {
                            mFinishedCreatingEglSurface = true;
                            VKSurfaceView.sGLThreadManager.notifyAll();
                        }
                    } else {
                        synchronized(VKSurfaceView.sGLThreadManager) {
                            mFinishedCreatingEglSurface = true;
                            mSurfaceIsBad = true;
                            VKSurfaceView.sGLThreadManager.notifyAll();
                        }
                        continue;
                    }
                    createEglSurface = false;
                }

//                if (createGlInterface) {
//                    VKSurfaceView view = mGLSurfaceViewWeakRef.get();
//                    gl = (GL10)view.mVKUtilsLib.createGL();
////                    gl = (GL10) mEglHelper.createGL();
//
//                    VKSurfaceView.sGLThreadManager.checkGLDriver(gl);
//                    createGlInterface = false;
//                }

                if (createEglContext) {
                    if (VKSurfaceView.LOG_RENDERER) {
                        Log.w("VKThread", "onSurfaceCreated");
                    }
                    VKSurfaceView view = mGLSurfaceViewWeakRef.get();
                    if (view != null) {
                        view.mRenderer.onSurfaceCreated(null);
                    }
                    createEglContext = false;
                }

                if (sizeChanged) {
                    if (VKSurfaceView.LOG_RENDERER) {
                        Log.w("VKThread", "onSurfaceChanged(" + w + ", " + h + ")");
                    }
                    VKSurfaceView view = mGLSurfaceViewWeakRef.get();
                    if (view != null) {
                        view.mRenderer.onSurfaceChanged( w, h);
                    }
                    sizeChanged = false;
                }

                if (VKSurfaceView.LOG_RENDERER_DRAW_FRAME) {
                    Log.w("VKThread", "onDrawFrame tid=" + getId());
                }
                {
                	VKSurfaceView view = mGLSurfaceViewWeakRef.get();
                    if (view != null) {
                        view.mRenderer.onDrawFrame();
                    }
                }
                Logger.printTime();
                VKSurfaceView view = mGLSurfaceViewWeakRef.get();
//                int swapError = view.mVKUtilsLib.swap();
//                int swapError = mEglHelper.swap();
                Logger.printTime();

                if (wantRenderNotification) {
                    doRenderNotification = true;
                }
            }

        } finally {
            /*
             * clean-up everything...
             */
            synchronized (VKSurfaceView.sGLThreadManager) {
//                stopEglSurfaceLocked();
//                stopEglContextLocked();
            }
        }
    }

    public boolean ableToDraw() {
        return mHaveEglContext && mHaveEglSurface && readyToDraw();
    }

    private boolean readyToDraw() {
        return (!mPaused) && mHasSurface && (!mSurfaceIsBad)
            && (mWidth > 0) && (mHeight > 0)
            && (mRequestRender || (mRenderMode == VKSurfaceView.RENDERMODE_CONTINUOUSLY));
    }

    public void setRenderMode(int renderMode) {
        if ( !((VKSurfaceView.RENDERMODE_WHEN_DIRTY <= renderMode) && (renderMode <= VKSurfaceView.RENDERMODE_CONTINUOUSLY)) ) {
            throw new IllegalArgumentException("renderMode");
        }
        synchronized(VKSurfaceView.sGLThreadManager) {
            mRenderMode = renderMode;
            VKSurfaceView.sGLThreadManager.notifyAll();
        }
    }

    public int getRenderMode() {
        synchronized(VKSurfaceView.sGLThreadManager) {
            return mRenderMode;
        }
    }

    public void requestRender() {
        synchronized(VKSurfaceView.sGLThreadManager) {
            mRequestRender = true;
            VKSurfaceView.sGLThreadManager.notifyAll();
        }
    }

    public void surfaceCreated() {
        synchronized(VKSurfaceView.sGLThreadManager) {
            if (VKSurfaceView.LOG_THREADS) {
                Log.i("VKThread", "surfaceCreated tid=" + getId());
            }
            mHasSurface = true;
            mFinishedCreatingEglSurface = false;
            VKSurfaceView.sGLThreadManager.notifyAll();
            while (mWaitingForSurface
                   && !mFinishedCreatingEglSurface
                   && !mExited) {
                try {
                		VKSurfaceView.sGLThreadManager.wait();
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            }
        }
    }

    public void surfaceDestroyed() {
        synchronized(VKSurfaceView.sGLThreadManager) {
            if (VKSurfaceView.LOG_THREADS) {
                Log.i("VKThread", "surfaceDestroyed tid=" + getId());
            }
            mHasSurface = false;
            VKSurfaceView.sGLThreadManager.notifyAll();
            while((!mWaitingForSurface) && (!mExited)) {
                try {
                	VKSurfaceView.sGLThreadManager.wait();
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            }
        }
    }

    public void onPause() {
        synchronized (VKSurfaceView.sGLThreadManager) {
            if (VKSurfaceView.LOG_PAUSE_RESUME) {
                Log.i("VKThread", "onPause tid=" + getId());
            }
            mRequestPaused = true;
            VKSurfaceView.sGLThreadManager.notifyAll();
            while ((! mExited) && (! mPaused)) {
                if (VKSurfaceView.LOG_PAUSE_RESUME) {
                    Log.i("Main thread", "onPause waiting for mPaused.");
                }
                try {
                	VKSurfaceView.sGLThreadManager.wait();
                } catch (InterruptedException ex) {
                    Thread.currentThread().interrupt();
                }
            }
        }
    }

    public void onResume() {
        synchronized (VKSurfaceView.sGLThreadManager) {
            if (VKSurfaceView.LOG_PAUSE_RESUME) {
                Log.i("VKThread", "onResume tid=" + getId());
            }
            mRequestPaused = false;
            mRequestRender = true;
            mRenderComplete = false;
            VKSurfaceView.sGLThreadManager.notifyAll();
            while ((! mExited) && mPaused && (!mRenderComplete)) {
                if (VKSurfaceView.LOG_PAUSE_RESUME) {
                    Log.i("Main thread", "onResume waiting for !mPaused.");
                }
                try {
                	VKSurfaceView.sGLThreadManager.wait();
                } catch (InterruptedException ex) {
                    Thread.currentThread().interrupt();
                }
            }
        }
    }

    public void onWindowResize(int w, int h) {
        synchronized (VKSurfaceView.sGLThreadManager) {
            mWidth = w;
            mHeight = h;
            mSizeChanged = true;
            mRequestRender = true;
            mRenderComplete = false;
            VKSurfaceView.sGLThreadManager.notifyAll();

            // Wait for thread to react to resize and render a frame
            while (! mExited && !mPaused && !mRenderComplete
                    && ableToDraw()) {
                if (VKSurfaceView.LOG_SURFACE) {
                    Log.i("Main thread", "onWindowResize waiting for render complete from tid=" + getId());
                }
                try {
                	VKSurfaceView.sGLThreadManager.wait();
                } catch (InterruptedException ex) {
                    Thread.currentThread().interrupt();
                }
            }
        }
    }

    public void requestExitAndWait() {
        // don't call this from VKThread thread or it is a guaranteed
        // deadlock!
        synchronized(VKSurfaceView.sGLThreadManager) {
            mShouldExit = true;
            VKSurfaceView.sGLThreadManager.notifyAll();
            while (! mExited) {
                try {
                	VKSurfaceView.sGLThreadManager.wait();
                } catch (InterruptedException ex) {
                    Thread.currentThread().interrupt();
                }
            }
        }
    }

    public void requestReleaseEglContextLocked() {
        mShouldReleaseEglContext = true;
        VKSurfaceView.sGLThreadManager.notifyAll();
    }

    /**
     * Queue an "event" to be run on the GL rendering thread.
     * @param r the runnable to be run on the GL rendering thread.
     */
    public void queueEvent(Runnable r) {
        if (r == null) {
            throw new IllegalArgumentException("r must not be null");
        }
        synchronized(VKSurfaceView.sGLThreadManager) {
            mEventQueue.add(r);
            VKSurfaceView.sGLThreadManager.notifyAll();
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

//    private EglHelper mEglHelper;

    /**
     * Set once at thread construction time, nulled out when the parent view is garbage
     * called. This weak reference allows the GLSurfaceView to be garbage collected while
     * the VKThread is still alive.
     */
    private WeakReference<VKSurfaceView> mGLSurfaceViewWeakRef;

}

