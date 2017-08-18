package com.baofeng.mojing;

import com.baofeng.mojing.MojingSDKReport;
import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.content.res.Configuration;
import android.os.Bundle;
import android.util.Log;

public class MojingVrActivity extends Activity {
    MojingSurfaceView mMojingSurfaceView;
    MojingSDKServiceManager mMojingSDKServiceManager;
    
    
    public MojingSurfaceView getSurfaceView()
    {
		return mMojingSurfaceView;
    }

    @Override protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mMojingSurfaceView = new MojingSurfaceView(this);
		mMojingSurfaceView.requestFocus();
		mMojingSurfaceView.setFocusableInTouchMode(true);
        setContentView(mMojingSurfaceView);
        
        mMojingSDKServiceManager = new MojingSDKServiceManager(this);
        
        /* select which version of OpenGL ES to use */
        final ActivityManager activityManager = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        final ConfigurationInfo configurationInfo = activityManager.getDeviceConfigurationInfo();
        boolean useGLES3 = true;
        if (configurationInfo.reqGlEsVersion >= 0x30000)
        {
        	useGLES3 = true;
        }
        else if (configurationInfo.reqGlEsVersion >= 0x20000)
        {
        	useGLES3 = false;
        }
        else
        {
        	Log.e("GLES", "neithor gles2 nor gles3 is supported");
        	System.exit(-1);
        }
        
		com.baofeng.mojing.MojingSDK.Init(this, useGLES3);
    }

    @Override protected void onPause() {
        mMojingSurfaceView.onPause();
        super.onPause();
        mMojingSDKServiceManager.onPause();
        com.baofeng.mojing.MojingVrLib.stopVsync(this);
		MojingSDKReport.onPause(this);
    }

    @Override protected void onResume() {
        super.onResume();   
        mMojingSDKServiceManager.onResume();
		com.baofeng.mojing.MojingVrLib.startVsync(this);
        mMojingSurfaceView.onResume();
		MojingSDKReport.onResume(this);
		//MojingSDKReport.onEvent("buy magic bean", "mojing shop", "money", 100, "magic bean", 1000);
    }
    
	@Override
	public void onConfigurationChanged(Configuration newConfig)
	{
		super.onConfigurationChanged(newConfig);
	}
}
