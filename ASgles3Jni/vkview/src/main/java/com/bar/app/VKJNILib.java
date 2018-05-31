package com.bar.app;

import android.content.res.AssetManager;
import android.view.Surface;

/**
 * Created by houguoli on 2018/5/31.
 */

public class VKJNILib {

    public void run(final Surface surface, final AssetManager assetmgr){
        new Thread(new Runnable() {
            @Override
            public void run() {
                initVK(surface, assetmgr);
                vkDrawFrame();
            }
        }).start();
    }
    public native void initVK(final Surface surface, final AssetManager assetmgr);
    public native void vkDrawFrame();
}
