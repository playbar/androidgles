package com.seu.magicfilter.filter.advanced;

import android.content.Context;
import android.opengl.GLES20;
import android.util.Log;

import com.seu.magicfilter.MagicEngine;
import com.seu.magicfilter.R;
import com.seu.magicfilter.filter.base.MagicLookupFilter;
import com.seu.magicfilter.filter.base.gpuimage.GPUImageFilter;
import com.seu.magicfilter.utils.OpenGlUtils;

public class MagicFairytaleFilter extends GPUImageFilter {

	protected String table;
	protected Context mContext;
	public int mLookupTextureUniform;
	public int mUniGlobalTime;
	public int mUniResolution;
	public int mUniRandPos;
	public int mLookupSourceTexture = OpenGlUtils.NO_TEXTURE;

	public MagicFairytaleFilter( ) {
		this( "filter/fairy_tale.png");
		mContext = MagicEngine.getContext();
	}

	public MagicFairytaleFilter( String table) {
		super(OpenGlUtils.readShaderFromRawResource( R.raw.fairytale_ver),
				OpenGlUtils.readShaderFromRawResource( R.raw.fairytale_fra));
		this.table = table;
	}



	protected void onInit(){
		super.onInit();
		mLookupTextureUniform = GLES20.glGetUniformLocation(getProgram(), "inputImageTexture2");
		mUniGlobalTime = GLES20.glGetUniformLocation( getProgram(), "uGlobalTime");
		mUniResolution = GLES20.glGetUniformLocation( getProgram(), "uResolution");
		mUniRandPos = GLES20.glGetUniformLocation(getProgram(), "uRandPos");
		return;
	}

	protected void onInitialized(){
		super.onInitialized();
		runOnDraw(new Runnable(){
			public void run(){
				mLookupSourceTexture = OpenGlUtils.loadTexture(mContext, table);
				float resolution =  0.6f; //(float)mSurfaceHeight / (float)mSurfaceWidth;
				GLES20.glUniform1f(mUniResolution, resolution);
			}
		});
	}

	protected void onDestroy(){
		super.onDestroy();
		int[] texture = new int[]{mLookupSourceTexture};
		GLES20.glDeleteTextures(1, texture, 0);
		mLookupSourceTexture = -1;
	}

	protected void onDrawArraysAfter(){
		if (mLookupSourceTexture != -1){
			GLES20.glActiveTexture(GLES20.GL_TEXTURE3);
			GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, 0);
			GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
		}
	}

	static float time = (float) 0.0;
	protected void onDrawArraysPre(){
		if( time > 3.14159 )
			time = (float) -3.14159;
		time += 0.07;
		Log.e("Fairytale", "time=" + time );
		GLES20.glUniform1f(mUniGlobalTime, time);
		//float x = ;
		//float y = ;
		//GLES20.glUniform2f( mUniRandPos, x, y );
		if (mLookupSourceTexture != -1){
			GLES20.glActiveTexture(GLES20.GL_TEXTURE3);
			GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mLookupSourceTexture);
			GLES20.glUniform1i(mLookupTextureUniform, 3);
		}
	}

}
