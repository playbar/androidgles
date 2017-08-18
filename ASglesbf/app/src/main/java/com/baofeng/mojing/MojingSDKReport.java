package com.baofeng.mojing;

import java.util.UUID;

import android.app.ActivityManager;
import android.content.Context;


public class MojingSDKReport {

	//private static boolean m_EnableEncrypt = false;
	private static boolean m_OpenDurationTrack = true;	
	private static String m_ActivityName = null;

	private static String getUUID() {	
		UUID uuid = UUID.randomUUID();
		return uuid.toString();
	} 

	private static String getRunningActivityName(Context context){  
		try
		{	        
			ActivityManager activityManager = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);  
			String runningActivity = activityManager.getRunningTasks(1).get(0).topActivity.getClassName();  //.getShortClassName(); //
			int index = runningActivity.lastIndexOf(".");
			if(index == -1)
			{
				return runningActivity;    
			}
			else
			{
				return runningActivity.substring(index+1);
			}  
		} catch (Exception e) {
			e.printStackTrace();
			return "UNKNOWN";
		}           
	}
	
/*
	public static void enableEncrypt(boolean enable)
	{
		m_EnableEncrypt = enable;
	}
*/	

	public static void setReportImmediate(boolean bImmediate)
	{
		com.baofeng.mojing.MojingSDK.AppSetReportImmediate(bImmediate);
	}

	public static void setReportInterval(int interval)
	{
		com.baofeng.mojing.MojingSDK.AppSetReportInterval(interval);
	}

	public static void setSessionContinueInterval(int interval)
	{
		com.baofeng.mojing.MojingSDK.AppSetContinueInterval(interval);
	}
	
	public static void openActivityDurationTrack(boolean enable)
	{
		m_OpenDurationTrack = enable;
	}

	public static void onResume(Context context)
	{		
		com.baofeng.mojing.MojingSDK.AppResume(getUUID());
		if(m_OpenDurationTrack)
		{
			//onPageStart(getRunningActivityName(context));
			m_ActivityName = getRunningActivityName(context);
			onPageStart(m_ActivityName);
		}
	}
	
	public static void onPause(Context context)
	{	
		com.baofeng.mojing.MojingSDK.AppPause();
		if(m_OpenDurationTrack)
		{
			//onPageEnd(getRunningActivityName(context));
			String name = getRunningActivityName(context);
			if(!name.equals(m_ActivityName))
			{
				//
			}
			onPageEnd(m_ActivityName);				
		}	
	}
	
	public static void onPageStart(String pageName)
	{			
		com.baofeng.mojing.MojingSDK.AppPageStart(pageName);	
	}
	
	public static void onPageEnd(String pageName)
	{	
		com.baofeng.mojing.MojingSDK.AppPageEnd(pageName);
	}
	
	public static void onEvent(String eventName, String eChannelID, String eInName, float eInData, String eOutName, float eOutData)
	{
		com.baofeng.mojing.MojingSDK.AppSetEvent(eventName, eChannelID, eInName, eInData, eOutName, eOutData);	
	}

	public static void onReportLog(int iLogType, String typeName, String logContent)
	{
		com.baofeng.mojing.MojingSDK.AppReportLog(iLogType, typeName, logContent);	
	}	
}
