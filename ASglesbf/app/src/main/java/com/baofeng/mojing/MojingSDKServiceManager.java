package com.baofeng.mojing;

import org.json.JSONException;
import org.json.JSONObject;

import com.baofeng.mojing.service.IServiceResponse;
import com.baofeng.mojing.service.MojingSDKAIDLService;
import com.baofeng.mojing.service.IServiceResponse.Stub;

import android.R.string;
import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;

public class MojingSDKServiceManager {

	final static String TAG = "MojingSDKServiceManager";
	
	private Activity mActivity;
	private MojingSDKAIDLService mService = null;
	
	private ServiceConnection connection = new ServiceConnection() {
		
        @Override
        public void onServiceDisconnected(ComponentName name) {
        	mService = null;
        }

        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            mService = MojingSDKAIDLService.Stub.asInterface(service);
        	try {
        		JSONObject jsonObj = new JSONObject();
				jsonObj.put("PackageName", mActivity.getPackageName());
				mService.clientResume(jsonObj.toString(), callback);
	        } catch (RemoteException e) {
	            e.printStackTrace();
	        } catch (JSONException e) {
	        	e.printStackTrace();
	        }
        }
    };
    
    private final IServiceResponse.Stub callback = new IServiceResponse.Stub() {
        public void sensorSuccess(final String params) {
			try {
				JSONObject jsonObj = new JSONObject(params);
				String glassName = jsonObj.getString("GlassName");
				com.baofeng.mojing.MojingSDK.StartGlassTracker(glassName);
			} catch (JSONException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				com.baofeng.mojing.MojingSDK.StartTracker(100);
			}
        }
        
        public void sensorFailed(final String params) {
        	com.baofeng.mojing.MojingSDK.StartTracker(100);
        }
    };
    
	public MojingSDKServiceManager(Activity activity)
	{
		mActivity = activity;
		
		Intent intent = new Intent();
		intent.setAction("com.baofeng.mojing.service.MojingSDKAIDLService");
		intent.setPackage("com.baofeng.mj");
		mActivity.startService(intent);
	}
	
	public void onResume() {
		Intent intent = new Intent();
		intent.setAction("com.baofeng.mojing.service.MojingSDKAIDLService");
		intent.setPackage("com.baofeng.mj");
		if (!mActivity.bindService(intent, connection, Context.BIND_AUTO_CREATE)) {
			Log.w(TAG, "ServiceApp 'com.baofeng.mj' not found!");
			com.baofeng.mojing.MojingSDK.StartTracker(100);
		}
	}
	
	public void onPause() {
		if (mService != null) {
			try {
				JSONObject jsonObj = new JSONObject();
				jsonObj.put("PackageName", mActivity.getPackageName());
				mService.clientPause(jsonObj.toString());
	        } catch (RemoteException e) {
	            e.printStackTrace();
	        } catch (JSONException e) {
	        	e.printStackTrace();
	        }
		}
		mActivity.unbindService(connection);
		
		com.baofeng.mojing.MojingSDK.StopTracker();
	}
}
