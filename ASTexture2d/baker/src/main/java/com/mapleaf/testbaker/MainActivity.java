package com.mapleaf.testbaker;

import android.os.Bundle;

import com.baofeng.mojing.MojingTextureBaker.MojingTextureBaker;
import com.baofeng.mojing.MojingTextureBaker.MojingTextureRender;
import com.google.vr.sdk.base.AndroidCompat;
import com.google.vr.sdk.base.GvrActivity;
import com.google.vr.sdk.base.GvrView;

public class MainActivity extends GvrActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        MojingTextureRender Render = new MojingTextureRender(this);
        Render.m_Baker = new MojingTextureBaker();

        GvrView gvrView = (GvrView) findViewById(R.id.gvr_view);
        gvrView.setEGLConfigChooser(8, 8, 8, 8, 16, 8);
        gvrView.setRenderer(Render);
        gvrView.setTransitionViewEnabled(true);
        gvrView.enableCardboardTriggerEmulation();
        if (gvrView.setAsyncReprojectionEnabled(true)) {
            AndroidCompat.setSustainedPerformanceMode(this, true);
        }
        setGvrView(gvrView);
    }
}
