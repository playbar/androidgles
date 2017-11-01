package com.baofeng.mojing.MojingTextureBaker;

import android.app.Activity;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;

import com.mapleaf.testbaker.R;

public class MojingTextureBaker extends Activity {
    private MojingGLSurfaceView mMojingSurfaceView = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main_baker);
        mMojingSurfaceView = (MojingGLSurfaceView) findViewById(R.id.gl_surfaceview);
        mMojingSurfaceView.setOnTouchListener(new View.OnTouchListener() {
            float previousX, previousY;
            @Override
            public boolean onTouch(View v, MotionEvent event) {

                if (event != null) {
                    if (event.getAction() == MotionEvent.ACTION_DOWN) {
                        previousX = event.getX();
                        previousY = event.getY();
                    } else if (event.getAction() == MotionEvent.ACTION_MOVE) {
                        final float deltaX = event.getX() - previousX;
                        final float deltaY = event.getY() - previousY;

                        previousX = event.getX();
                        previousY = event.getY();

                        mMojingSurfaceView.queueEvent(new Runnable() {
                            @Override
                            public void run() {
                                mMojingSurfaceView.handleTouchDrag(
                                        deltaX, deltaY);
                            }
                        });
                    }

                    return true;
                } else {
                    return false;
                }
            }
        });
    }

    public static native int BeginBaker(int iMaxSize);
    public static native int AddTexture(int hHandle, int iTextureID, int index, int iX, int iY, int iWidth, int iHeight);
    public static native int RemoveTexture(int hHandle, int iTextureID,int index, int iX, int iY, int iWidth, int iHeight);
    public static native int GetBakerTextureSize(	int hHandle);
    public static native String BakerTexture(int hHandle, int iTextureID, boolean bReplaceByBakedTexture);
    public static native void EndBaker(int hHandle);

    static {
        System.loadLibrary("MojingTextureBaker");
    }
}