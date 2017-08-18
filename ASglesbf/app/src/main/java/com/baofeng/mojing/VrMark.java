package com.baofeng.mojing;

import java.util.Timer;
import java.util.TimerTask;

import android.content.Context;

public class VrMark {

	public static int ScreenMark(Context context, String glassKey)
	{
        MojingSDK.Init(context, true);
		android.util.DisplayMetrics dm = context.getResources().getDisplayMetrics();
		int r = (int) (2 * MojingSDK.GetScreenPPI() * 100.0 * MojingSDK.GetDistortionR(glassKey) / 2.54 + 0.5); 
		if (r > 0)
		{
			return (Math.min(r, Math.max(dm.widthPixels,dm.heightPixels)/2) * Math.min(r, Math.min(dm.widthPixels,dm.heightPixels)) + 100)/200;
		}
		else
		{
			return ((Math.max(dm.widthPixels,dm.heightPixels)/2) * Math.min(dm.widthPixels,dm.heightPixels) + 100)/200;
		}
	}
	
	public static float m_fRate;// 0...100
	public static float[] m_Samples = new float[500];
	// SAMPLE OFFSET
	public static float m_fMaxContiguousOffset;		
	public static float m_fMaxSampleOffset;			
	public static float m_fAvgSampleOffset;			
			
	public static float m_fMaxContiguousAngle;		
	public static float m_fMaxSampleAngle;			
	public static float m_fMaxContiguousTimeSpace;

	public static float m_fAvfTemperature;			
	public static float m_fTemperatureOffset;		
	public static float m_fTrackerScore;
	public static int StartTrackerMark(int nSampleFrequence)
	{
		m_fRate = -1;
		if (StartTrackerChecker(nSampleFrequence) > 0)
		{
			m_fRate = 0;
			
			Timer t = new Timer();
			t.schedule(new TimerTask(){

				@Override
				public void run() {
					float[] Params = new float[8];
					float fPI = 3.1415926535898f;
					// TODO Auto-generated method stub
					m_fRate = GetTrackerCheckerResult(m_Samples , Params) / 500;
					m_fMaxContiguousOffset = Params[0];		
					m_fMaxSampleOffset = Params[1];			
					m_fAvgSampleOffset = Params[2];			
							
					m_fMaxContiguousAngle = Params[3] / fPI * 180;		
					m_fMaxSampleAngle = Params[4] / fPI * 180;			
					m_fMaxContiguousTimeSpace = Params[5]*1000*1000;

					m_fAvfTemperature = Params[6];			
					m_fTemperatureOffset = Params[7];	
					
					if (m_fRate >= 1)
						this.cancel();
				}
				
			}, 50);
			return 1;// ok
		}//end of if
		return 0;
	}// StartTrackerMark

	private static native int StartTrackerChecker(int nSampleFrequence);
	private static native int GetTrackerCheckerResult(float[] Samples , float[] Parmats);
}
