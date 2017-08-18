package com.bn.mojing;

import java.io.IOException;
import java.io.InputStream;
import java.io.Serializable;

import android.app.Application;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PixelFormat;
import android.graphics.Typeface;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;

import com.baofeng.mojing.MojingSurfaceView;
import com.bn.mojing.GL2JNIView;
import com.bn.mojing.MatrixState;
import com.bn.mojing.SphereModel;

import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.GLUtils;
import android.opengl.Matrix;

import java.nio.IntBuffer;
import java.util.Calendar;


import android.util.DisplayMetrics;

import android.util.*;
import java.lang.Math;

class GL2JNIView extends MojingSurfaceView implements Serializable{
    Renderer renderer;
    static int g_TexId = 0;
    static Context mContext;    
    
    static int g_textureImgWidth = 800;
    static int g_textureImgHeight = 800;
	// static int g_sphereTexId;
    
    static int g_frameBufferObject = 0;

	

    public GL2JNIView(Context context) {
        super(context);
        super.setTimeWarp(true);
        super.setMultiThread(true);

        String MJ2 = "F79F2H-Q8ZNXN-2HQN2F-2ZA9YV-QG4H9H-QGAYAE";
		String MJ3 = "CY42HN-FKCRQ8-SGS34S-8K23ZR-SZSBQ8-QFXF95";
		String MJ3_88pmma = "8349QZ-YBAQA8-QNWW86-W8ADQH-WVDFSG-FV8NHN";
		String MJ3_96pmma = "SFCNDH-4WCYA4-4W8NX8-ZN9NF8-C386HT-A49ZWB";

        // we pass the surface to the SDK for building OpenGL context
		// 
		super.setGlassesKey(MJ3);
        
		mContext = context;
		renderer=new Renderer();
		this.setRenderer(renderer);
		this.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
    }

        public void onPause()
    {
    	super.onPause();
//    	renderer.EndOfRender = true;
    }
    
    public void onResume()
    {
//    	renderer.EndOfRender = false;
    	super.onResume();
    }
    
//    private static class Renderer implements GLSurfaceView.Renderer {
//    	SphereModel sphere;
//    	boolean EndOfRender = false;
//		int iFreamCount = 0;
//		private Bitmap bitmap;
//		Canvas canvas;
//        public void onDrawFrame(GL10 gl) {	 	
//
//			//renderToWindow();
//            
//			renderToTexture();        	
//			//renderTextureToWindow();
//
//        	// com.baofeng.mojing.MojingSDK.DrawTexture(g_sphereTexId, 0);
//			//com.baofeng.mojing.MojingSDK.DrawTexture(0, 0);
//        	}
//
//        public void onSurfaceChanged(GL10 gl, int width, int height) {
//			com.baofeng.mojing.MojingSDK.SetCenterLine(4, 255,255,255,255);
//			com.baofeng.mojing.MojingSDK.LogTrace("EnterMojingWorld");
//			
//            float fFOV = com.baofeng.mojing.MojingSDK.GetMojingWorldFOV(); 
//
//            float ratio = (float) Math.tan(Math.toRadians(fFOV/2)) * 1.0f;
//            MatrixState.setProjectFrustum(-ratio, ratio, -ratio, ratio, 1.f, 800);
//            MatrixState.setCamera(0,0,0,		
//								  0f,0.0f,-0.1f,
//								  0f,1.0f,0.0f);  
//			//com.baofeng.mojing.MojingSDK.OnSurfaceChanged(width , height);
//
//			float ff = com.baofeng.mojing.MojingSDK.GetGlassesSeparationInPix();
//        }
//
//        public void onSurfaceCreated(GL10 gl, EGLConfig config) 
//        {            
//			android.util.DisplayMetrics display = mContext.getResources().getDisplayMetrics();	
//			
//			if (display.widthPixels > display.heightPixels)
//			{
//				g_textureImgWidth  = display.widthPixels;
//				g_textureImgHeight = display.heightPixels;
//			}
//			else
//			{
//				g_textureImgWidth  = display.heightPixels;
//				g_textureImgHeight = display.widthPixels;
//			}
//            MatrixState.setInitStack();
//
//            GLES20.glClearColor(0.0f,0.0f,0.0f, 1.0f);  
//            GLES20.glEnable(GLES20.GL_DEPTH_TEST);
//        	
//			g_TexId = initImageTexture();	
//        	// g_sphereTexId = generateSphereTexture();
//        	g_frameBufferObject= generateFrameBufferObject();
//
//			sphere = new SphereModel(100);    
//
//			com.baofeng.mojing.MojingSDK.SetCenterLine(4, 255,255,255,255);
//			com.baofeng.mojing.MojingSDK.LogTrace("EnterMojingWorld");
//
//			com.baofeng.mojing.MojingSDKPrivate.FuncTest();
//
//
//			com.baofeng.mojing.MojingSDK.StartTrackerCalibration();
//        }
//        
//        
//        public static int initImageTexture() {
//    		
//            InputStream is = mContext.getResources().openRawResource(R.drawable.strip1);//mContext.getResources().openRawResource(R.drawable.strip);
//			//InputStream is = mContext.getResources().openRawResource(R.drawable.bmp123);
//            Bitmap bitmap;
//            bitmap = BitmapFactory.decodeStream(is);
//            
//            int[] textures = new int[1];
//    		GLES20.glGenTextures(1, textures, 0);
//    		
//    		int textureId=textures[0];    
//    		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureId);
//
//			GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER,GLES20.GL_LINEAR);
//    		GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER,GLES20.GL_LINEAR);
//    		GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S,GLES20.GL_CLAMP_TO_EDGE);
//    		GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T,GLES20.GL_CLAMP_TO_EDGE);
//
//    		     
//            GLUtils.texImage2D( GLES20.GL_TEXTURE_2D, 0, bitmap, 0);
//            
//           	bitmap.recycle(); 		  
//
//            return textureId;
//    	}
//    	
//    	private static int generateSphereTexture() {
//        	IntBuffer renderTexture = IntBuffer.allocate(1);        	
//        	int texWidth = g_textureImgWidth / 2;
//			int texHeight = g_textureImgWidth / 2; 	
//    	
//        	GLES20.glGenTextures(1, renderTexture);
//        	
//        	GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, renderTexture.get(0));
//
//        	// parameters - we have to make sure we clamp the textures to the edges
//        	GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S,
//        			GLES20.GL_CLAMP_TO_EDGE);
//        	GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T,
//        			GLES20.GL_CLAMP_TO_EDGE);
//        	GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER,
//        			GLES20.GL_LINEAR);
//        	GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER,
//        			GLES20.GL_LINEAR);
//        	
//        	// generate the textures
//        	GLES20.glTexImage2D(GLES20.GL_TEXTURE_2D, 0, GLES20.GL_RGB, texWidth, texHeight, 0, GLES20.GL_RGB, GLES20.GL_UNSIGNED_BYTE, null);
//        	
//        	GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, 0);
//        	
//        	return renderTexture.get(0);
//    	}
//    	
//    	private static int generateFrameBufferObject() {
//    		IntBuffer framebuffer = IntBuffer.allocate(1);
//    		GLES20.glGenFramebuffers(1, framebuffer); 
//    		
//    		return framebuffer.get(0);
//    	}
//    	
//    	
//		//int count = 0;
//    	public void renderToWindow()
//    	{
//			GLES20.glClearColor(0,0,0,1);
//			GLES20.glClear( GLES20.GL_DEPTH_BUFFER_BIT | GLES20.GL_COLOR_BUFFER_BIT);
//			int texWidth = g_textureImgWidth, texHeight = g_textureImgHeight;
//			GLES20.glViewport(0, 0, texWidth, texHeight);    
//            // save the matrix
//            MatrixState.pushMatrix(); 
//
//			float[] fM  = new float[16];
//		    com.baofeng.mojing.MojingSDK.getLastHeadView(fM);
//	        MatrixState.setViewMatrix(fM);
//            sphere.drawSelf(g_TexId);
//            MatrixState.popMatrix();   
//
//    	}
//		public void renderTextureToWindow()
//    	{
//			GLES20.glClearColor(0,0,0,1);
//			GLES20.glClear( GLES20.GL_DEPTH_BUFFER_BIT | GLES20.GL_COLOR_BUFFER_BIT);
//			int texWidth = g_textureImgWidth, texHeight = g_textureImgHeight;
//			GLES20.glViewport(0, 0, texWidth, texHeight);    
//            // save current context.
//            com.baofeng.mojing.MojingSDK.DrawTexture(g_TexId, 0); 
//    	}
//
//    	public void renderToTexture() {	
//
//        	int EyeTex[] = {0 , 0};
//			float Camera[] = {-0.1f , 0.1f};
//			for (int iEye = 0 ; iEye < 2 ; iEye++)
//			{
//				GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, g_frameBufferObject);
//				com.baofeng.mojing.EyeTextureParameter EyeTexture = com.baofeng.mojing.MojingSDK.GetEyeTextureParameter(iEye + 1);
//			
//				EyeTex[iEye] = EyeTexture.m_EyeTexID;
//				if (EyeTex[iEye] != 0 && GLES20.glIsTexture(EyeTex[iEye]))
//        		{
//					GLES20.glFramebufferTexture2D(GLES20.GL_FRAMEBUFFER, GLES20.GL_COLOR_ATTACHMENT0, GLES20.GL_TEXTURE_2D, EyeTexture.m_EyeTexID, 0);
//					
//        			GLES20.glClearColor(0,0,0,1);
//        			GLES20.glViewport(0, 0, EyeTexture.m_Width, EyeTexture.m_Height);        	        	
//					GLES20.glClear(GLES20.GL_DEPTH_BUFFER_BIT | GLES20.GL_COLOR_BUFFER_BIT);   
//					
//					// GLES20.glViewsport(1, 1, EyeTexture.m_Width - 2,  EyeTexture.m_Height - 2);        	        	
//
//		        	int status = GLES20.glCheckFramebufferStatus(GLES20.GL_FRAMEBUFFER);
//        			if (status == GLES20.GL_FRAMEBUFFER_COMPLETE)
//        			{
//        				GLES20.glClearColor(0,0,0,1);
//						GLES20.glClear(GLES20.GL_DEPTH_BUFFER_BIT | GLES20.GL_COLOR_BUFFER_BIT);   
//                
//						MatrixState.pushMatrix();
//
//						MatrixState.setCamera(Camera[iEye],0,0,		
//										  0f,0.0f,-0.1f,
//										  0f,1.0f,0.0f);  
//
//						float[] fM  = new float[16];
//						com.baofeng.mojing.MojingSDK.getLastHeadView(fM);
//	        			MatrixState.setViewMatrix(fM);
//						sphere.drawSelf(g_TexId);
//						MatrixState.popMatrix(); 
//                
//        			}  // endi if (status == GLES20.GL_FRAMEBUFFER_COMPLETE)    	
//				}// endi if (EyeTex[iEye] != 0)
//        		GLES20.glFramebufferTexture2D(GLES20.GL_FRAMEBUFFER,GLES20.GL_COLOR_ATTACHMENT0,GLES20.GL_TEXTURE_2D,0,0);
//				GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER,0);
//			}// end for 
//			
//			com.baofeng.mojing.MojingSDK.DrawTexture(EyeTex[0], EyeTex[1]);
//			//com.baofeng.mojing.MojingSDK.DrawTexture(0, 0);
//    	}
//    } // end of Render
    
    private static class Renderer implements GLSurfaceView.Renderer
    {
        public void onDrawFrame(GL10 gl)
        {
            float[] sensorView = {
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            };
            com.baofeng.mojing.MojingSDK3288.RenderFrame(sensorView);
        }

        public void onSurfaceChanged(GL10 gl, int width, int height)
        {
            float[] modelView = {
                -0.5f, 0.0f, 0.0f, // leftCameraPos
                0.5f, 0.0f, 0.0f, // rightCameraPos
                0.0f, 0.0f, -1.0f, // lookAt
                0.0f, 1.0f, 0.0f // upVec
                            };
            float[] perspective = {
                1.5707963268f, 1.0f, 0.1f, 100.0f, // left(fov, w/h, near, far)
                1.5707963268f, 1.0f, 0.1f, 100.0f // right
            };

            com.baofeng.mojing.MojingSDK3288.RenderInit(width, height, 0, modelView, perspective);
        }

        public void onSurfaceCreated(GL10 gl, EGLConfig config)
        {

        }
    }
}
