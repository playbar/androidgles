package com.baofeng.mojing;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.res.AssetManager;
import android.content.res.Resources;



public class MojingSDKPrivate
{
//	static 
//	 {
//         System.loadLibrary("mojing");
//     }
	
	private static native void Log(int logLevel, String sInfo, String sFileName, int line);

	private static void Log(int logLevel, String sInfo)
	{
		StackTraceElement[] stacktrace = Thread.currentThread().getStackTrace();
		// 0 is dalvik.system.VMStack.getThreadStackTrace
		// 1 is java.lang.Thread.getStackTrace
		// 2 is this function: Log
		// 3 is the caller
		String tag = "[" + stacktrace[4].getMethodName() + "] " + sInfo;
		Log((int)logLevel, tag, stacktrace[4].getFileName(), stacktrace[4].getLineNumber());
      }


	
	public static native void FuncTest();
	public static native boolean DistortionSetLensSeparation(float fNewValue);
	public static native float DistortionGetLensSeparation();
	
	public static native boolean DistortionSetParamet(int iSegment, float[] jKR, float[] jKG, float[] jKB);
	public static native int DistortionGetParamet(float[] jKR, float[] jKG, float[] jKB);
	public static native boolean DistortionSaveParamet();
	public static native boolean DistortionResetParamet();
	// Get CPU/GPU name
	public static native String GetCpuName();
	public static native String GetGpuName();

	  public static void LogError(String sInfo)
	  {
		Log(40000, sInfo);
	  }

	  public static void LogWarning(String sInfo)
	  {
		Log(30000, sInfo);
	  }

	  public static void LogDebug(String sInfo)
	  {
		Log(10000, sInfo);
	  }

  	  public static void LogTrace(String sInfo)
	  {
		Log(0, sInfo);
	  }
	  
}
