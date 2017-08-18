package com.baofeng.mojing;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.LineNumberReader;
import java.io.OutputStream;
import java.lang.reflect.Method;
import java.util.Set;
import java.util.Timer;
import java.util.TimerTask;
import java.util.UUID;

import android.R.bool;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.telephony.TelephonyManager;
import android.view.InputDevice;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageInfo;
import android.content.res.AssetManager;
import android.content.res.Resources;

public class MojingSDK
{
    public static int 	SENSOR_ERROR_NOERROR		= 0;
	public static int 	SENSOR_ERROR_NoMag			=0x01;
	public static int 	SENSOR_ERROR_NoGryo			=0x04;
	public static int 	SENSOR_ERROR_GryoTooSlow	=0x08;
	public static int 	SENSOR_ERROR_NoAccel		=0x10;
	public static int 	SENSOR_ERROR_AccelTooSlow	=0x20;

	public static int	EYE_TEXTURE_TYPE_LEFT =	1;
	public static int	EYE_TEXTURE_TYPE_RIGHT=	2;
	public static int	EYE_TEXTURE_TYPE_BOTH =	3;
	private static Timer g_DeviceTimer;
	
//	static 
//	{
//		System.loadLibrary("mdklog");
//		System.loadLibrary("mojing");
//    }

	public static native int GetSystemIntProperty(String property, int defaultValue);

	private static boolean m_inited = false;
	private static native boolean Init(String MerchantID, String AppID, String AppKey, String AppName, String packageName, String userID, String channelID, int nWidth, int nHeight, float xdpi, float ydpi, String ProfilePath);
	public static boolean Init(Context context, boolean useGLES3)
	{	
		if (!m_inited)
		{
			System.loadLibrary("mdklog");
			if (useGLES3)
			{
				System.loadLibrary("mojing_gles3");
			}
			else
			{
				System.loadLibrary("mojing_gles2");
			}
				
			String path = exportFromAssetsFile(context);//context.getApplicationContext().getPackageResourcePath();
			String appName = getApplicationName(context);	
			String packageName = getAppPackageName(context);
			String userID = getUserID(context);
			String channelID = getCustomMetaData(context, "DEVELOPER_CHANNEL_ID");
			String appID = getCustomMetaData(context, "DEVELOPER_APP_ID");
			String appKey = getCustomMetaData(context, "DEVELOPER_APP_KEY");
			String merchantID = getCustomMetaData(context, "DEVELOPER_MERCHANT_ID");
			android.util.DisplayMetrics dm = context.getResources().getDisplayMetrics();	
			m_inited = true;
			return Init(merchantID, appID, appKey, appName, packageName, userID, channelID, dm.widthPixels,dm.heightPixels,  dm.xdpi , dm.ydpi, path);
		}
		return true;
	}
   
	public static EyeTextureParameter GetEyeTextureParameter(int eyeTextureType)
	{ 
	   EyeTextureParameter Ret =  new EyeTextureParameter();
		int []Parameter = {0,0,0};
		Ret.m_EyeTexID = GetEyeTexture(eyeTextureType , Parameter);
		Ret.m_EyeTexType=eyeTextureType ;
		Ret.m_Width = Parameter[0];
		Ret.m_Height= Parameter[1];
		Ret.m_Format= Parameter[2];
		return Ret;
   }

   private static String exportFromAssetsFile(Context context)
   {
   	String result=null;    	
	File CacheDir = context.getCacheDir();
   	String cachePath = CacheDir.getPath();
   	result = cachePath;
	File cachePathFile = new File(cachePath);
	if (cachePathFile == null)
	{
		//	LogTrace("Can not open cache floder at : " + cachePath);
	}
	else
	{
		if (!cachePathFile.exists() || !cachePathFile.isDirectory())
		{
			cachePathFile.mkdir();
		}
	}
		// LogTrace("Cache : " + cachePath + " , Exists = " + cachePathFile.exists() + " , Dir = " + cachePathFile.isDirectory());

   	try
   	{
   		Resources r = context.getResources();
   		String[] ProfileList =r.getAssets().list("MojingSDK"); 
   		LogTrace("Find " + ProfileList.length + " file(s) in assets/MojingSDK");
   		if (ProfileList.length > 0)
   		{
   			AssetManager assetManager = r.getAssets();
   			for(String filename : ProfileList) {
   				InputStream in = null;
   				OutputStream out = null;
   				try {
   					in = assetManager.open("MojingSDK/" + filename);
   					File outFile = new File(CacheDir, filename);
   					out = new FileOutputStream(outFile);
   					copyFile(in, out);
   					in.close();
   					in = null;
   					out.flush();
   					out.close();
   					out = null;
   					LogTrace("copy " + filename + " to CacheDir");
   				} catch(IOException e) {
   					LogError("Failed to copy asset file: " + filename + e.toString());
   				}// end catch       
   			}// end for
   		}//length>0
   		
   	}// end try
   	catch(Exception e)
   	{
   		e.printStackTrace();
   	}   
	return result;
   }
   
   private static void copyFile(InputStream in, OutputStream out) throws IOException 
   {
		byte[] buffer = new byte[1024];
		int read;
		while((read = in.read(buffer)) != -1)
		{
			out.write(buffer, 0, read);
		}
	}

	/**
	 * get CPU serial No.
	 * @return CPU serial No.(16 bytes)
	 *  if failed, return "0000000000000000"
	 */
	private static String getCPUSerial() 
	{
		String str = "", strCPU = "", cpuAddress = "0000000000000000";
		try {
			//read cpu info
			Process pp = Runtime.getRuntime().exec("cat /proc/cpuinfo");
			InputStreamReader ir = new InputStreamReader(pp.getInputStream());
			LineNumberReader input = new LineNumberReader(ir);

			for (int i = 1; i < 100; i++) {
				str = input.readLine();
				if (str != null) {
					// read "serial" field
					if (str.indexOf("Serial") > -1) {
						strCPU = str.substring(str.indexOf(":") + 1, str.length());
						cpuAddress = strCPU.trim();
						break;
					}
				} else {
					//eof 
					break;
				}
			}
		} catch (IOException ex) {
			ex.printStackTrace();
		}
		
		return cpuAddress;
	}

	public static String getSerialNumber(){ 

	    String serial = "0000000000000000"; 

	    try { 

	    Class<?> c =Class.forName("android.os.SystemProperties"); 

	       Method get =c.getMethod("get", String.class); 

	       serial = (String)get.invoke(c, "ro.serialno"); 

	    } catch (Exception e) { 

	       e.printStackTrace(); 

	    } 

	    return serial; 

	} 

	private static String getUserID(Context context) {	
		String uniqueId = "UNKNOWN";
		try {
			final TelephonyManager tm = (TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE);
			final String tmDevice, androidId,cpuSerial,serial;

			tmDevice = "" + tm.getDeviceId();
			androidId = "" + android.provider.Settings.Secure.getString(
						context.getContentResolver(),
						android.provider.Settings.Secure.ANDROID_ID);
			//cpuSerial = "" + getCPUSerial();
			serial = getSerialNumber();
			UUID deviceUuid = new UUID(androidId.hashCode(), ((long) tmDevice.hashCode() << 32) |serial.hashCode());
			uniqueId = deviceUuid.toString();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return uniqueId;
	} 

	public static String getCustomMetaData(Context context, String metadataKey) {
		String metadataValue = "UNKNOWN";
		try {
	        PackageManager packageManager = context.getPackageManager();
	        ApplicationInfo applicationInfo = packageManager.getApplicationInfo(context.getPackageName(), PackageManager.GET_META_DATA); 
			Object obj = (Object)applicationInfo.metaData.get(metadataKey);
			if(obj != null)
			{
				metadataValue = obj.toString();
			}
		} catch (Exception e) {
			e.printStackTrace();
			metadataKey = "";
		}

		return metadataValue;
	}
   
	private static String getAppPackageName(Context context) {  
		String packageName = "UNKNOWN";

        //PackageInfo packageInfo = null;  
        try {  
			String pkName = context.getPackageName();
 			//packageInfo = context.getPackageManager().getPackageInfo(pkName, 0);
			packageName = pkName; // + packageInfo.versionName + packageInfo.versionCode; 
        } catch (Exception e) {  
        	e.printStackTrace(); 
        }  

        return packageName;  
    }

	private static String getApplicationName(Context context) {  
		String applicationName = "UNKNOWN";
        PackageManager packageManager = null;  
        ApplicationInfo applicationInfo = null;  
        try {  
            packageManager = context.getPackageManager();
            applicationInfo = packageManager.getApplicationInfo(context.getApplicationInfo().packageName, 0);  
			applicationName = (String) packageManager.getApplicationLabel(applicationInfo); 
			
			PackageInfo packInfo = packageManager.getPackageInfo(context.getPackageName(),0);  
			String version = packInfo.versionName;  
			int versionCode = packInfo.versionCode;  
			if (version != null)
				applicationName = applicationName + " " + version + "("+versionCode + ")";
			else
				applicationName = applicationName + " (" + versionCode + ")";
        } catch (PackageManager.NameNotFoundException e) {  
            applicationInfo = null;  
        }  
        return applicationName;   
    }  

	public static native void AppExit();
	public static native boolean AppResume(String uniqueID);
	public static native void AppPause();
	public static native void AppPageStart(String pageName);
	public static native void AppPageEnd(String pageName);
	public static native void AppSetEvent(String eventName, String eChannelID, String eInName, float eInData, String eOutName, float eOutData);
	public static native void AppReportLog(int iLogType, String typeName, String logContent);
	public static native void AppSetContinueInterval(int interval);
	public static native void AppSetReportInterval(int interval);
	public static native void AppSetReportImmediate(boolean bImmediate);

	////////////////////////////////////////////////////////////////
	public static native void OnKeyEvent(String deviceName, int buttonId, boolean buttonDown);
	public static native void OnAxisEvent(String deviceName, int AxisID, float value);
	
	// API FOR SENSORS
	public static native boolean StartTracker(int nSampleFrequence);
	public static native boolean StartGlassTracker(String glassName);
	public static native int CheckSensors();
	public static native void ResetSensorOrientation();
	public static native void ResetTracker();
	public static native float IsTrackerCalibrated();
	public static native boolean StartTrackerCalibration();
	public static native void getLastHeadView(float[] headView);
	public static native int  getPredictionHeadView(float[] headView, double time);
	
	public static native void getLastHeadEulerAngles(float[] headEulerAngles);
	public static native void getLastHeadQuarternion(float[] jUnitQuarternion);
	public static native void StopTracker();

	// API FOR DISPLAY/DISTORTION
	public static native boolean DrawTexture(int LeftTexID, int RightTexID);	
	// public static native boolean DrawTextureWithSameOverlay(int LeftTexID, int RightTexID, int OverlayID);
	public static native boolean DrawTextureWithOverlay(int LeftTexID, int RightTexID, int LeftOverlayID, int RightOverlayID);

	public static native void SetOverlayPosition(float fLeft, float fTop, float fWidth, float fHeight);
	public static native void SetOverlayPosition3D(int eyeTextureType, float fWidth, float fHeight, float fDistanceInMetre);
	public static native float GetMojingWorldFOV();	
	private static native int GetEyeTexture(int eyeTextureType, int[] outParams);
	public static native float GetGlassesSeparationInPix();
	public static native void SetImageYOffset(float fYOffset);
	public static native void SetCenterLine(int iWidth , int colR, int colG, int colB, int colA);
	public static void SetCenterLine(int iWidth )
	{
		SetCenterLine(iWidth , 255 , 255 ,255 , 255);
	}
	// API FOR GLASSES MANAGER
	public static native String GetManufacturerList(String strLanguageCodeByISO639);
	public static native String GetProductList(String strManufacturerKey, String strLanguageCodeByISO639);
	public static native String GetGlassList(String  strProductKey, String  strLanguageCodeByISO639);
	public static native String GetGlassInfo(String  strGlassKey, String  strLanguageCodeByISO639);
	public static native String GenerationGlassKey(String  strProductQRCode, String  strGlassQRCode);
	public static native boolean SetDefaultMojingWorld(String strGlassesKey);
	public static native String GetDefaultMojingWorld(String strLanguageCodeByISO639);
	public static native String GetLastMojingWorld(String strLanguageCodeByISO639);

	public static native String GetSDKVersion();
	public static native boolean GetInitSDK();
	public static native boolean GetStartTracker();
	public static native boolean GetInMojingWorld();
	public static native String GetGlasses();
	public static native float GetScreenPPI();
	public static native float GetDistortionR(String strGlassesKey);
	public static native String GetUserSettings();
	public static native boolean SetUserSettings(String strUserSettings);

	// Mojing5 API
	public static native float GetLightSensation();
	public static native float GetProximitySensation();
	
	// Joystatic ?? 
	public static native String GetJoystickFileName();
	public static native void NativeSetMojing2Number(int iNumber);
	private static native void NativeCleanDeviceMap();
	private static native void NativeBeginUpdateDeviceMap();
	private static native void NativeEndUpdateDeviceMap();
	private static native void NativeAddDeviceToMap(int jiDeviceID, String jsDeviceName);
	
	public static class KTXInfo
	{
		public int iTextureID = 0;
		public int iTarget = 0;
		public int iHeight = 0;
		public int iWidth = 0;
		public int iDepth = 0;
		public boolean bIsMipmapped;
		public int iGLError = 0;
		public int iKTXError = 0;
		public KTXInfo()
		{
			
		}
	}
	
	public static native boolean NativektxLoadTextureN(String jsPath, KTXInfo info);
	// SDK Interface For Unreal
	public static void onNativeActivePause()
	{
		if (g_DeviceTimer != null)
			g_DeviceTimer.cancel();		

		g_DeviceTimer = null;
	}
  

	private static Object g_DeviceTimerSync = new Object();
	public static void onNativeActiveResume()
	{
		if (g_DeviceTimer == null)
			g_DeviceTimer = new Timer();
		g_DeviceTimer.schedule(new TimerTask()
		{
			@Override
			public void run()
			{
				synchronized (g_DeviceTimerSync)
				{
					// Test if Disconnect is called!
					if (g_DeviceTimer != null)
					{
						// Find within all input devices
						NativeBeginUpdateDeviceMap();
						int[] temp = InputDevice.getDeviceIds();
						for(int i = 0; i < temp.length; i++) 
						{
							InputDevice inputDevice = InputDevice.getDevice(temp[i]);
							////////////////////////////////////////////////////////
							int deviceID = inputDevice.getId();
							String strDeviceName = inputDevice.getName();
							////////////////////////////////////////////////////////
							NativeAddDeviceToMap(deviceID , strDeviceName);
						}
						NativeEndUpdateDeviceMap();
					}// end of if
				}// end of synchronized (g_DeviceTimerSync)
			}// end of run
		}, 0, 5000);
	}
	
	//////////////////////////////////////////////////////////////////////////////
	public static native void FuncTest();
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
