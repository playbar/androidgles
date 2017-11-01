package com.baofeng.mojing.MojingTextureBaker;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.util.Log;

/**
 * Created by zhangxingang on 2017/9/11.
 */

public class MojingGLSurfaceView extends GLSurfaceView{
    private MojingGLCubemapRender mCubemapRender;
    private Context mContext;
    public MojingGLSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        mContext = context;
        try{
            setEGLContextClientVersion(2);
            mCubemapRender = new MojingGLCubemapRender(mContext);
            setRenderer(mCubemapRender);
            setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
        }catch (Exception e){
            Log.e("MojingGLSurfaceView", "Unable to create GLSurfaceView context!", e);
            e.printStackTrace();
        }
    }

    public void handleTouchDrag(float deltaX, float deltaY) {
        mCubemapRender.handleTouchDrag(deltaX, deltaY);
    }
}
