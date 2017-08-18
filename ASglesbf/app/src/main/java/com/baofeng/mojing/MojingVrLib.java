package com.baofeng.mojing;

import android.app.Activity;
import android.view.Choreographer;
import android.view.Choreographer.FrameCallback;

import com.baofeng.mojing.MojingSDK;

public class MojingVrLib implements android.view.Choreographer.FrameCallback
{
//	static 
//	 {
//        System.loadLibrary("mojing");
//	 }

  	public static MojingVrLib lib = new MojingVrLib();
  	public static Choreographer choreographerInstance;
	
  	public static void startVsync( Activity act )
  	{
      	act.runOnUiThread( new Thread()
      	{
  		 @Override
      		public void run()
      		{
  				choreographerInstance = Choreographer.getInstance();
  				choreographerInstance.removeFrameCallback((FrameCallback) lib);
  				m_FrameCount = 0;
  				choreographerInstance.postFrameCallback((FrameCallback) lib);
      		}
      	});
  		
  	}

  	public static void stopVsync( Activity act ) 
  	{
  		// This may not need to be run on the UI thread, but it doesn't hurt.
      	act.runOnUiThread( new Thread()
      	{
  		 @Override
      		public void run()
      		{
  				choreographerInstance.removeFrameCallback((FrameCallback) lib);
      		}
      	});		
  	}

  	private static int m_FrameCount = 0;
  	private static native void nativeVsync(long lastVsyncNano);
  	public void doFrame(long frameTimeNanos) 
  	{
  		m_FrameCount++;
  		// MojingSDK.LogDebug(String.format("doFrame %d at %d", m_FrameCount, frameTimeNanos));
  		nativeVsync(frameTimeNanos);
  		choreographerInstance.postFrameCallback((FrameCallback) this);
  	}
}
