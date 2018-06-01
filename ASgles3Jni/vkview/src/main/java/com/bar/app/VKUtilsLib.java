package com.bar.app;

import android.content.res.AssetManager;
import android.util.Log;
import android.view.Surface;

/**
 * Created by houguoli on 2018/5/31.
 */

public class VKUtilsLib {

    private static final String VERTEX_SHADER = "shaders/triangle.vert.spv";
    private static final String FRAGMENT_SHADER = "shaders/triangle.frag.spv";

    static {
        System.loadLibrary("vkutils");
    }

    public void run(final Surface surface, final AssetManager assetmgr){
        new Thread(new Runnable() {
            @Override
            public void run() {
                initVK(surface, assetmgr);
                vkDrawFrame();
            }
        }).start();
    }

    private long mNativeVulanUtils;

    public native void initVK(final Surface surface, final AssetManager assetmgr);
    public native void vkDrawFrame();

    ////

    public VKUtilsLib(AssetManager assetManager){
        mNativeVulanUtils = nativeCreate(assetManager, VERTEX_SHADER, FRAGMENT_SHADER);
    }

    public VKUtilsLib(AssetManager assetManager, String vertexShader, String fragmentShader) {
        mNativeVulanUtils = nativeCreate(assetManager, vertexShader, fragmentShader);
    }

    private static native long nativeCreate(AssetManager assetManager, String vertexShader,
                                      String fragmentShader);

    private static native void nativeRun(long nativeHandle, Surface surface);

    private static native void nativePause(long nativeHandle);

    private static native void nativeResume(long nativeHandle);

    private static native void nativeSurfaceChanged(long nativeHandle);

    private static native void nativeStop(long nativeHandle);

    public void run(final Surface surface) {
//        new Thread(new Runnable() {
//            @Override
//            public void run() {
                Log.e("java run", "tid=" + android.os.Process.myTid());
                VKUtilsLib.nativeRun(mNativeVulanUtils, surface);
//            }
//        }).start();
    }

    public void pause() {
        nativePause(mNativeVulanUtils);
    }

    public void resume() {
        nativeResume(mNativeVulanUtils);
    }

    public void surfaceChanged() {
        nativeSurfaceChanged(mNativeVulanUtils);
    }

    public void stop() {
        nativeStop(mNativeVulanUtils);
    }


}
