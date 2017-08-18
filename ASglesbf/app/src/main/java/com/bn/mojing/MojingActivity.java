package com.bn.mojing;

import java.io.Serializable;
import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.List;

import com.baofeng.mojing.MojingSurfaceView;
import com.baofeng.mojing.MojingVrActivity;

import android.content.Intent;
import android.content.res.Configuration;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.util.SparseArray;
import android.view.KeyEvent;
import android.view.MotionEvent;


public class MojingActivity extends MojingVrActivity{
	static final int SEND_SMS_REQUEST = 0; 
	static final int CALL_REQUEST = 1; 
	
   static int StartCount = 0;
	SparseArray<String> stateMap = new SparseArray<String>();
	
    MojingSurfaceView mView;

    static String MJ2 = "F79F2H-Q8ZNXN-2HQN2F-2ZA9YV-QG4H9H-QGAYAE";
	static String MJ3 = "CY42HN-FKCRQ8-SGS34S-8K23ZR-SZSBQ8-QFXF95";
	static String MJ3_88pmma = "8349QZ-YBAQA8-QNWW86-W8ADQH-WVDFSG-FV8NHN";
	static String MJ3_96pmma = "SFCNDH-4WCYA4-4W8NX8-ZN9NF8-C386HT-A49ZWB";
	static String MJ4 = "2WF5F5-FPWGZZ-H7AE2C-H3F8SW-EE8KCF-YTHBCN";
	static String MJA1="WNE8QN-QVCCZN-XXF8W3-9Q2YDH-AQZRA2-XVDZ9Q";
	static String MJ_D="Z74QHR-AC9RYL-4FZGQ3-QGZZ2H-9T86Q5-XGFVW8";
	static String UC_VR = "WZ2Z8K-9VFT2Y-ZZQHZB-Z82N48-2CZL4B-CGAZDW";
	static String lefant = "Y7XVWU-4BDSFN-S99HA8-8598WR-YVH78U-Y7XNSW";
	static String MJ5 = "9GEDFU-954ESX-QV9B2Y-YVXNEG-FLDWAY-ZFS99Z";
	static String MJ_RIO = "SXF8DB-ZUSQS8-A2FMDC-SHHLHB-SZZNY3-DHC8Y7";
	

	//static String[] KeyList = {MJ2,MJ3,MJ3_96pmma,MJ3_88pmma,MJ4,MJ_RIO,MJ5};

	static String Test1 = "9ZQRW8-CH9T98-WZYYF8-ZZY3DW-9YDWQ8-WGFUWU";
	static String Test2 = "8KYB29-QTA8EE-95YB9B-28FUAA-AHA22W-2FCQSH";
	static String Test3 = "ZR95FU-HTA84C-YN2NDW-9H8R2X-D8WGH8-SZWNFM";
	static String Test4 = "FBEYH7-YG8LCH-4823EZ-QBCFD8-CBZ598-8KWVCC";
	static String Test5 = "HH23QR-EH83CC-SWS3EH-Q7CVWQ-YVSHX8-WBSSSH";
	static String Test6 = "EXFPZN-ST8BS2-Q34HH5-AXC3E8-298RFF-8G44Q3";
	static String Test7 = "ZBED2F-FFS849-F7CYEF-QNE2XX-S9QY9V-SWCW86";
	static String Test8 = "ZHSXEF-EX48QR-2WQ8YF-4DY7Q3-4Y8BAS-87CZST";
	static String Test9 = "AHAS9Z-DB4CE9-QNCFEZ-48Z5ZH-HRSTWQ-2YFRX8";
	static String Test10 = "DTYHAD-WQACWV-QYEZEW-S34CAW-DT9H9Y-SHYVA8";
	static String Test11 = "WUAC4Z-SGCYDZ-DFZKAF-CNQRED-8NQ8FR-9R8KQF";
	static String[] KeyList = {
	Test1 			,
	Test2 			,
	Test3 			,
	Test4 			,
	Test5 			,
	Test6 			,
	Test7 			,
	Test8 			,
	Test9 			,
	Test10			,
	Test11			
	};




	static String VrBox = "WYAQWH-CF95YZ-WWHL4Y-ZBE9YT-QZWGWT-9VDGWH";
	static String QIKU360="WUZFSH-XX8393-8RDY9B-XCS9WW-CBH8ZK-Q599XC";//奇酷360
	static String MOKE="X9DDQ5-XG9RDC-ADX3XF-DFHLHT-YTXWSQ-ZUHVWY";// 莫克
	static String QHMJ="9FWZDH-4SSXEH-8UZTFR-QYEH4Z-2XSFWQ-H8F59G";// 千幻魔镜

	static boolean   bTimeWarp = true;
	static boolean   bMultiThread = true;
	static String DefaultGlass = MJ4;
	private VrPhotoRender renderer;
	
	private Handler handler = new Handler( );

    private Runnable runnable = new Runnable( ) {
    	public void run ( ) {
//	    	Log.e("TEST", "GetLightSensation:" + com.baofeng.mojing.MojingSDK.GetLightSensation() + " GetProximitySensation:" + com.baofeng.mojing.MojingSDK.GetProximitySensation());
		    handler.postDelayed(this,1000);
	    }
    };
	
	GLMsgHandler mGLMsgHandler;
	private static class GLMsgHandler extends Handler {
		private final WeakReference<MojingActivity> mActivity;

		public GLMsgHandler(MojingActivity activity) {
			mActivity = new WeakReference<MojingActivity>(activity);
		}

		@Override
		public void handleMessage(Message msg) {
			MojingActivity activity = mActivity.get();
			if (activity != null) {
				if (msg.what == MojingSurfaceView.ON_CHANGE_MOJING_WORLD) 
				{
					activity.onChangeMojingWorld();
				}
			}
		}
	}
	
	public void onChangeMojingWorld() 
	{
		float fov = com.baofeng.mojing.MojingSDK.GetMojingWorldFOV() / 2.f;            
        float ratio = (float)Math.tan(Math.toRadians(fov));
        MatrixState.setProjectFrustum(-ratio, ratio, -ratio, ratio, 1.f, 800);
	}
	
    @Override protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        mView = super.getSurfaceView();
        
        mGLMsgHandler = new GLMsgHandler(this);
        mView.setMessageHandler(mGLMsgHandler);
        
        mView.setTimeWarp(bTimeWarp);
        mView.setMultiThread(bMultiThread);
        mView.setGlassesKey(DefaultGlass);

		renderer=new VrPhotoRender(this);
		mView.setRenderer(renderer);
		mView.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
        
		String sUserSetting = com.baofeng.mojing.MojingSDK.GetUserSettings();
		sUserSetting = "{\"ClassName\":\"UserSettingProfile\",\"EnableScreenSize\":0,\"ScreenSize\":9}";
		com.baofeng.mojing.MojingSDK.SetUserSettings(sUserSetting);

		String  strManufacturerList = 
			com.baofeng.mojing.MojingSDK.GetManufacturerList("ZH");
		String  strProductList = 
			com.baofeng.mojing.MojingSDK.GetProductList("E92YDY-AHDQWV-ACF39Y-QT4EDC-CZ4CFU-4E8B85", "ZH");
		
		String  strGlassList = 
			com.baofeng.mojing.MojingSDK.GetGlassList("SG2W2B-HG95SX-29CR8M-EFZ8DQ-9HEZW3-2BYZZU", "ZH");
		
		String  strGlassInfo = 
			com.baofeng.mojing.MojingSDK.GetGlassInfo("SFCNDH-4WCYA4-4W8NX8-ZN9NF8-C386HT-A49ZWB", "ZH");
		
		String  strGenerationGlassKey = 
			com.baofeng.mojing.MojingSDK.GenerationGlassKey("E92YDY-AHDQWV-ACF39Y-QT4EDC-CZ4CFU-4E8B85",  "CY42HN-FKCRQ8-SGS34S-8K23ZR-SZSBQ8-QFXF95");
		
		com.baofeng.mojing.MojingSDK.LogTrace("strManufacturerList >>>>" + strManufacturerList);
		com.baofeng.mojing.MojingSDK.LogTrace("strProductList >>>>" + strProductList);
		com.baofeng.mojing.MojingSDK.LogTrace("strGlassList >>>>" + strGlassList);
		com.baofeng.mojing.MojingSDK.LogTrace("strGlassInfo >>>>" + strGlassInfo);

		strManufacturerList = 
			com.baofeng.mojing.MojingSDK.GetManufacturerList("ZH");
		strProductList = 
			com.baofeng.mojing.MojingSDK.GetProductList("E92YDY-AHDQWV-ACF39Y-QT4EDC-CZ4CFU-4E8B85", "ZH");
		
		strGlassList = 
			com.baofeng.mojing.MojingSDK.GetGlassList("SG2W2B-HG95SX-29CR8M-EFZ8DQ-9HEZW3-2BYZZU", "ZH");
		
		strGlassInfo = 
			com.baofeng.mojing.MojingSDK.GetGlassInfo("SFCNDH-4WCYA4-4W8NX8-ZN9NF8-C386HT-A49ZWB", "ZH");
		
		strGenerationGlassKey = 
			com.baofeng.mojing.MojingSDK.GenerationGlassKey("E92YDY-AHDQWV-ACF39Y-QT4EDC-CZ4CFU-4E8B85",  "CY42HN-FKCRQ8-SGS34S-8K23ZR-SZSBQ8-QFXF95");
		
		com.baofeng.mojing.MojingSDK.LogTrace("strManufacturerList 2 >>>>" + strManufacturerList);
		com.baofeng.mojing.MojingSDK.LogTrace("strProductList 2 >>>>" + strProductList);
		com.baofeng.mojing.MojingSDK.LogTrace("strGlassList 2 >>>>" + strGlassList);
		com.baofeng.mojing.MojingSDK.LogTrace("strGlassInfo 2 >>>>" + strGlassInfo);

    }

    
	    
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		
		if (event.getAction() == MotionEvent.ACTION_UP)
		{
			//jump2AdjustActivity();
			String MojingType = KeyList[StartCount];
			mView.setGlassesKey(MojingType);
			//com.baofeng.mojing.MojingSDK.ChangeMojingWorld(MojingType);
			StartCount  = StartCount + 1;
			StartCount %=  KeyList.length;
		}	
		
		return super.onTouchEvent(event);
	}
    
    
	@Override protected void onDestroy() {	
		
		super.onDestroy();
		com.baofeng.mojing.MojingSDKPrivate.DistortionSaveParamet();
		com.baofeng.mojing.MojingSDK.LogTrace("MojingActivity onDestroy");	
	}

    @Override protected void onPause() {
        //com.baofeng.mojing.MojingVrLib.stopVsync(this);
        super.onPause();
        //mView.onPause();
		//com.baofeng.mojing.MojingSDK.StopTracker();
		
		//MojingSDKReport.onPause(this);
        
//        handler.removeCallbacks(runnable);
    }

    @Override protected void onResume() {
        super.onResume();   
       // mView.onResume();

        //ZGameBTUtil.setMzKeyCallback(this);
        
		//int ii = com.baofeng.mojing.MojingSDK.CheckSensors();
		//com.baofeng.mojing.MojingSDK.StartTracker(100);
		//ii = com.baofeng.mojing.MojingSDK.CheckSensors();
		//com.baofeng.mojing.MojingVrLib.startVsync(this);
		//MojingSDKReport.onResume(this);
		//MojingSDKReport.onEvent("buy magic bean", "mojing shop", "money", 100, "magic bean", 1000);
        
//        handler.postDelayed(runnable, 1000);
    }
    
    private void jump2AdjustActivity() {
		// mView.SetEndFream();
		// Intent intent = new Intent(this, AdjustActivity.class);
		
		// List parameter = new ArrayList();
		// for(int i = 0; i < param.length; i++)
		// {
		// 	parameter.add(param[i]);
		// }
		// Bundle bundle = new Bundle();
		// bundle.putSerializable("parameter", (Serializable) parameter);		
		// intent.putExtras(bundle);
		
		//startActivityForResult(intent, SEND_SMS_REQUEST);
		// startActivity(intent);	
		finish();
    }
    
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
    	switch (resultCode) 
    	{
    		case RESULT_OK:
    	    break;
    	default:
    	    break;
    	    }
    	}
    
  

	@Override
	public void onConfigurationChanged(Configuration newConfig)
	{
		super.onConfigurationChanged(newConfig);
	}

}
