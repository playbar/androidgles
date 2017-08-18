package com.bn.mojing;

import java.io.InputStream;
import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Typeface;
import android.opengl.GLSurfaceView;
import android.util.Log;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import com.baofeng.mojing.MojingSDK;
import com.bn.mojing.MatrixState;
import com.bn.mojing.SphereModel;

import android.opengl.GLES20;
import android.opengl.GLES30;
import android.opengl.GLUtils;
import java.nio.IntBuffer;
import java.lang.Math;

public class VrPhotoRender implements GLSurfaceView.Renderer
{
	public VrPhotoRender(Activity activity)
	{
		//this.mojingSurfaceView = mojingSurfaceView;
//		mContext = activity;
	}
    public void onDrawFrame(GL10 gl)
    {
//		float[] sensorView =
//		{
//			1.0f, 0.0f, 0.0f, 0.0f,
//			0.0f, 1.0f, 0.0f, 0.0f,
//			0.0f, 0.0f, 1.0f, 0.0f,
//			0.0f, 0.0f, 0.0f, 1.0f
//		};

		float[] fM  = new float[16];
		com.baofeng.mojing.MojingSDK.getLastHeadView(fM);
        com.baofeng.mojing.MojingSDK3288.RenderFrame(fM);
    }

    public void onSurfaceChanged(GL10 gl, int width, int height)
    {
		float[] modelView = {-0.5f, 0.0f, 0.0f, // leftCameraPos
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

//public class VrPhotoRender implements GLSurfaceView.Renderer {
//    	SphereModel sphere;
//    	SkyBoxModel SkyBox;
//    	boolean EndOfRender = false;
//		int iFreamCount = 0;
//		private Bitmap bitmap;
//		Canvas canvas;
//		
//		Activity mContext;
//		private int g_textureImgWidth;
//		private int g_textureImgHeight;
//		private int g_TexId;
//		private int g_SkyboxId;
//		private int g_LeftTexLayout = 0;
//		private int g_RightTexLayout = 0;
//		private int g_frameBufferObject;
//		private float g_Radius = 400;
//	    
//		public VrPhotoRender(Activity activity)
//		{
//			//this.mojingSurfaceView = mojingSurfaceView;
//			mContext = activity;
//		}
//		
//        public void onDrawFrame(GL10 gl) {
//
//			//renderToWindow();
////        	if (EndOfRender)
////        	{
////        		com.baofeng.mojing.MojingSDK.LeaveMojingWorld();
////				EndOfRender =false;
////        		return;
////        	}
//        	
//
//			//float fRate = com.baofeng.mojing.MojingSDK.IsTrackerCalibrated();
//			//renderToWindow();
//            //initFontBitmap();
//			renderToTexture();        	
//			//renderTextureToWindow();
//
//        	// com.baofeng.mojing.MojingSDK.DrawTexture(g_sphereTexId, 0);
//			// com.baofeng.mojing.MojingSDK.DrawTexture(0, 0);
//        	}
//
//        public void onSurfaceChanged(GL10 gl, int width, int height) {
//			com.baofeng.mojing.MojingSDK.SetCenterLine(4, 255,255,255,255);
//			com.baofeng.mojing.MojingSDK.LogTrace("EnterMojingWorld");
//			
//            float fFOV = com.baofeng.mojing.MojingSDK.GetMojingWorldFOV(); 
//			//float fFOV = 90; 
//            float ratio = (float) Math.tan(Math.toRadians(fFOV/2)) * 1.0f;
//            Log.e("Mojing", String.format("radio is %f", ratio));
//			float fNear = g_Radius * 0.1f;
//			float fFar = g_Radius *3f;// 注意：天空盒模式下，从中心点到斜角的距离较远
//            MatrixState.setProjectFrustum(-ratio * fNear, ratio * fNear, -ratio * fNear, ratio * fNear, fNear, fFar);
//            MatrixState.setCamera(0,0,0,		
//								  0f,0.0f,-0.1f,
//								  0f,1.0f,0.0f);  
//			//com.baofeng.mojing.MojingSDK.OnSurfaceChanged(width , height);
//
//			float ff = com.baofeng.mojing.MojingSDK.GetGlassesSeparationInPix();
//        }
//
//        
//        
//        public void onSurfaceCreated(GL10 gl, EGLConfig config) 
//        {
//			android.util.DisplayMetrics display = mContext.getResources().getDisplayMetrics();	
//			
//            // we pass the surface to the SDK for building OpenGL context
//			//
//			// com.baofeng.mojing.MojingSDK.EnterMojingWorld(MJ3, true);
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
//			g_TexId = initImageTexture(R.drawable.strip1);
//			g_SkyboxId = initImageTexture(R.drawable.skybox);
//			g_LeftTexLayout= initImageTexture(R.drawable.star);
//			g_RightTexLayout= initImageTexture(R.drawable.bird);
//        	// g_sphereTexId = generateSphereTexture();
//        	g_frameBufferObject= generateFrameBufferObject();
//
//			sphere = new SphereModel(g_Radius);    
//			sphere.CreateVBO();
//
//
//			// com.baofeng.mojing.MojingSDKPrivate.FuncTest();
//			SkyBox = new SkyBoxModel(g_Radius , 1.0f/202.0f);
//			SkyBox.CreateVBO();
//			// com.baofeng.mojing.MojingSDK.StartTrackerCalibration();
//			
//			com.baofeng.mojing.MojingSDK.SetCenterLine(4, 255,255,255,255);
//			com.baofeng.mojing.MojingSDK.LogTrace("EnterMojingWorld");
//        }
//        
//        
//        public void initFontBitmap(){  
//            String font = "闇�瑕佹覆鏌撶殑鏂囧瓧娴嬭瘯锛�";  
//            bitmap = Bitmap.createBitmap(256, 256, Bitmap.Config.ARGB_8888);  
//            canvas = new Canvas(bitmap);  
//            // backcolor 
//            canvas.drawColor(Color.LTGRAY);  
//            Paint p = new Paint();  
//            //set the font type
//            String fontType = "瀹嬩綋";  
//            Typeface typeface = Typeface.create(fontType, Typeface.BOLD);  
//            // enable anti alias鈻�  
//            p.setAntiAlias(true);  
//            // font color 
//            p.setColor(Color.RED);  
//            p.setTypeface(typeface);  
//            p.setTextSize(28);  
//            // draw text銆� 
//            canvas.drawText(font, 0, 100, p);  
//        }  
//
//    	public int initImageTexture(int iResourceID) {
//    		
//            InputStream is = mContext.getResources().openRawResource(iResourceID);//mContext.getResources().openRawResource(R.drawable.strip);
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
//    	private int generateSphereTexture() {
//        	IntBuffer renderTexture = IntBuffer.allocate(1);        	
//        	int texWidth = g_textureImgWidth;
//			int texHeight = g_textureImgWidth; 	
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
//			// save the matrix.            MatrixState.pushMatrix(); 
//
//			float[] fM  = new float[16];
//		    com.baofeng.mojing.MojingSDK.getLastHeadView(fM);
//	        MatrixState.setViewMatrix(fM);
//            //sphere.drawSelf(g_TexId);
//		   sphere.Draw(g_TexId);
//          //   MatrixState.popMatrix();   
//
//    	}
//		public void renderTextureToWindow()
//    	{
//			GLES20.glClearColor(0,0,0,1);
//			GLES20.glClear( GLES20.GL_DEPTH_BUFFER_BIT | GLES20.GL_COLOR_BUFFER_BIT);
//			int texWidth = g_textureImgWidth, texHeight = g_textureImgHeight;
//			GLES20.glViewport(0, 0, texWidth, texHeight);    
//			// save current context.
//            com.baofeng.mojing.MojingSDK.DrawTexture(g_TexId, 0); 
//    	}
//
//		float[] fM  = new float[16];
//		int iFrameCount = 0;
//    	public void renderToTexture() {
//			int iMaxRender = 2;
//			boolean bUsingLayer = true;
//			//boolean bUsingLayer = false;//disenable star layer
//        	int EyeTex[] = {0 , 0};
//			float Camera[] = {-1f , 1f};
//			
//			
//			for (int iEye = 0 ; iEye < iMaxRender ; iEye++)
//			{
//				//Log.e("Draw Eye", "Start Eye: " + iEye);
//				GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, g_frameBufferObject);
//				com.baofeng.mojing.EyeTextureParameter EyeTexture = com.baofeng.mojing.MojingSDK.GetEyeTextureParameter(iEye + 1);
//			
//				EyeTex[iEye] = EyeTexture.m_EyeTexID;
//				
//				if (EyeTex[iEye] != 0 && GLES20.glIsTexture(EyeTex[iEye]))
//        		{
//					GLES20.glFramebufferTexture2D(GLES20.GL_FRAMEBUFFER, GLES20.GL_COLOR_ATTACHMENT0, GLES20.GL_TEXTURE_2D, EyeTexture.m_EyeTexID, 0);
//					// 1. clear the buffer with black.
//        			//GLES20.glClearColor(0,0,0,1);
//        			GLES20.glViewport(0, 0, EyeTexture.m_Width, EyeTexture.m_Height);        	        	
//					//GLES20.glClear(GLES20.GL_DEPTH_BUFFER_BIT | GLES20.GL_COLOR_BUFFER_BIT);   
//					// 2. set view port
//					// GLES20.glViewsport(1, 1, EyeTexture.m_Width - 2,  EyeTexture.m_Height - 2);        	        	
//
//		        	int status = GLES20.glCheckFramebufferStatus(GLES20.GL_FRAMEBUFFER);
//        			if (status == GLES20.GL_FRAMEBUFFER_COMPLETE)
//        			{
//        				GLES20.glClearColor(0,0,0,1);
//						//GLES20.glClear(GLES20.GL_DEPTH_BUFFER_BIT | GLES20.GL_COLOR_BUFFER_BIT);   
//                
//						//MatrixState.pushMatrix();
//
//						MatrixState.setCamera(0,0,200,		
//										  0f,0.0f,-0.1f,
//										  0f,1.0f,200.0f);  
//
//						com.baofeng.mojing.MojingSDK.getLastHeadView(fM);
//	        			MatrixState.setViewMatrix(fM);
//						//sphere.drawSelf(g_TexId);
//						// sphere.Draw(g_TexId);
//						//MatrixState.popMatrix(); 
//						SkyBox.Draw(g_SkyboxId);
//        			}  // endi if (status == GLES20.GL_FRAMEBUFFER_COMPLETE)    	
//				}// endi if (EyeTex[iEye] != 0)
//        		
//				GLES20.glFramebufferTexture2D(GLES20.GL_FRAMEBUFFER,GLES20.GL_COLOR_ATTACHMENT0,GLES20.GL_TEXTURE_2D,0,0);
//				GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER,0);
//				
//				//Log.e("Draw Eye", "End Eye: " + iEye);
//			}// end for 
//			
//			com.baofeng.mojing.MojingSDK.SetOverlayPosition(0.0f,0.0f,1,1);
//			com.baofeng.mojing.MojingSDK.DrawTextureWithOverlay(EyeTex[0], EyeTex[iMaxRender-1]  , g_LeftTexLayout, 0);
//
//			//com.baofeng.mojing.MojingSDK.DrawTexture(EyeTex[0], EyeTex[iMaxRender-1]  );
//
//		//	com.baofeng.mojing.MojingSDK.DrawTexture(0,0);
//			
////			float fFrame = iFrameCount;
////
////			if ( bUsingLayer && iFrameCount > 100)
////			{
////				com.baofeng.mojing.MojingSDK.SetOverlayPosition(0.0f,0.0f,1.0f,1.0f);
////				com.baofeng.mojing.MojingSDK.DrawTextureWithOverlay(EyeTex[0], EyeTex[iMaxRender-1]  , g_LeftTexLayout, g_RightTexLayout);
////			}
////			else
////			{
////				com.baofeng.mojing.MojingSDK.SetOverlayPosition3D(3 , 0.04f , 0.04f , 2.0f);
////				com.baofeng.mojing.MojingSDK.DrawTextureWithOverlay(EyeTex[0], EyeTex[iMaxRender-1]  , g_LeftTexLayout, g_LeftTexLayout);
////			}
////
////			iFrameCount = (iFrameCount + 1) % 200;
////			if (iFrameCount == 50)
////			{
////				String sUserSetting = "{\"ClassName\":\"UserSettingProfile\",\"EnableScreenSize\":0,\"ScreenSize\":9}";
////				com.baofeng.mojing.MojingSDK.SetUserSettings(sUserSetting);
////			}if (iFrameCount == 150)
////			{
////				String sUserSetting = "{\"ClassName\":\"UserSettingProfile\",\"EnableScreenSize\":1,\"ScreenSize\":6.7}";
////				com.baofeng.mojing.MojingSDK.SetUserSettings(sUserSetting);
////			}
//			
//    	}
//    }
