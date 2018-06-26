package com.android.app;

import android.app.Application;
import android.content.ComponentCallbacks;
import android.content.res.Configuration;
import android.util.Log;

import com.android.hook.GLESHook;


public class App extends Application {

	static {
        System.loadLibrary("glhook");
	}

	private String textData = "default";
	private String TAG = "Application";

	public void setTextData(String textData) {
		this.textData = textData;
	}

	public String getTextData() {
		return textData;
	}

	@Override
	public void onCreate() {
		// 程序创建的时候执行
		Log.d(TAG, "onCreate");
		super.onCreate();
		GLESHook.initHook();

	}
	@Override
	public void onTerminate() {
		// 程序终止的时候执行
		Log.d(TAG, "onTerminate");
		GLESHook.unInitHook();
		super.onTerminate();
	}
	@Override
	public void onLowMemory() {
		// 低内存的时候执行
		Log.d(TAG, "onLowMemory");
		super.onLowMemory();
	}
	@Override
	public void onTrimMemory(int level) {
		// 程序在内存清理的时候执行
		Log.d(TAG, "onTrimMemory");
		super.onTrimMemory(level);
	}
	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		Log.d(TAG, "onConfigurationChanged");
		super.onConfigurationChanged(newConfig);
	}

	@Override
	public void registerActivityLifecycleCallbacks(
			ActivityLifecycleCallbacks callback) {
		Log.d(TAG, "registerActivityLifecycleCallbacks");
		super.registerActivityLifecycleCallbacks(callback);
	}

	@Override
	public void registerComponentCallbacks(ComponentCallbacks callback) {
		Log.d(TAG, "registerComponentCallbacks");
		super.registerComponentCallbacks(callback);
	}

	@Override
	public void unregisterActivityLifecycleCallbacks(
			ActivityLifecycleCallbacks callback) {
		Log.d(TAG, "unregisterActivityLifecycleCallbacks");
		super.unregisterActivityLifecycleCallbacks(callback);
	}

	@Override
	public void unregisterComponentCallbacks(ComponentCallbacks callback) {
		Log.d(TAG, "unregisterComponentCallbacks");
		super.unregisterComponentCallbacks(callback);
	}

}
