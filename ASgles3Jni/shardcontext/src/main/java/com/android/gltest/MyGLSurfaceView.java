package com.android.gltest;

import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.util.Log;
 
/**
 *
 * @author gaofeng
 *
 */
public class MyGLSurfaceView extends GLSurfaceViewAll {
     
    private GLRenderer renderer;
 
    /**
     * @param context
     */
    public MyGLSurfaceView(Context context) {
        super(context);
        init();
    }
 
    /**
     * @param context
     * @param attrs
     */
    public MyGLSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }
     
    private void init() {
        ActivityManager am = (ActivityManager) getContext().getSystemService(Context.ACTIVITY_SERVICE);
        ConfigurationInfo info = am.getDeviceConfigurationInfo();
        String v = info.getGlEsVersion(); //判断是否为3.0 ，一般4.4就开始支持3.0版本了。
        if (v.equalsIgnoreCase("3.0")) {
            setEGLContextClientVersion(3);
        } else {
            setEGLContextClientVersion(2);
        }
        renderer = new GLRenderer();
        renderer.setContext(getContext());
        setRenderer(renderer);
        setRenderMode(RENDERMODE_CONTINUOUSLY);
    }
}
