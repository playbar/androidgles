package com.android.gles3jni;

import java.io.IOException;
import java.io.InputStream;
import javax.microedition.khronos.opengles.GL10;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.Bitmap.Config;
import android.opengl.GLES11Ext;
import android.opengl.GLES30;
import android.opengl.GLUtils;
import android.util.Log;


/**
 * 
 * ClassName: GLTextureUtils <br/>
 * @author lixianke    
 * @date: 2015-3-9 下午1:51:58 <br/>  
 * description:
 */
public class GLTextureUtils {
	public static boolean mUseMipMap = false;

	public static int initImageTexture(Context context, int drawableId) {
		//通过输入流加载图片===============begin===================
        InputStream is = context.getResources().openRawResource(drawableId);
        Bitmap bitmap;
        try 
        {
        	bitmap = BitmapFactory.decodeStream(is);
        } 
        finally 
        {
            try 
            {
                is.close();
            } 
            catch(IOException e) 
            {
                e.printStackTrace();
            }
        }
        //通过输入流加载图片===============end=====================  
        
        return initImageTexture( bitmap, true);
	}
	
	public static int initImageTexture( Bitmap bm, boolean isRecycle) {
		if (bm == null) {
			return -1;
		}
		
		//生成纹理ID
		int[] textures = new int[1];
		
		GLES30.glGenTextures
		(
			1,          //产生的纹理id的数量
			textures,   //纹理id的数组
			0           //偏移量
		); 
		Log.e("GLTextureUtils", "texrId=" + textures[0]);
		
		if (mUseMipMap) {
			GLES30.glEnable(GLES30.GL_TEXTURE_2D);
			GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, textures[0]);
			GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MIN_FILTER,GLES30.GL_LINEAR_MIPMAP_LINEAR);
			GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D,GLES30.GL_TEXTURE_MAG_FILTER,GLES30.GL_LINEAR_MIPMAP_LINEAR);
			GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_S,GLES30.GL_CLAMP_TO_EDGE);
			GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_T,GLES30.GL_CLAMP_TO_EDGE);
	        //实际加载纹理       
	        GLUtils.texImage2D
	        (
	        		GLES30.GL_TEXTURE_2D,   //纹理类型，在OpenGL ES中必须为GL10.GL_TEXTURE_2D
	        		0, 					  //纹理的层次，0表示基本图像层，可以理解为直接贴图
	        		bm, 			  //纹理图像
	        		0					  //纹理边框尺寸
	        );
	        GLES30.glGenerateMipmap(GLES30.GL_TEXTURE_2D); 
		} else {
			GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, textures[0]);
			GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MIN_FILTER,GLES30.GL_LINEAR);
			GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MAG_FILTER,GLES30.GL_LINEAR);
			GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_S,GLES30.GL_CLAMP_TO_EDGE);
			GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_T,GLES30.GL_CLAMP_TO_EDGE);
	        //实际加载纹理       
	        GLUtils.texImage2D
	        (
	        		GLES30.GL_TEXTURE_2D,   //纹理类型，在OpenGL ES中必须为GL10.GL_TEXTURE_2D
	        		0, 					  //纹理的层次，0表示基本图像层，可以理解为直接贴图
	        		bm, 			  //纹理图像
	        		0					  //纹理边框尺寸
	        );
		}
        
        if (isRecycle){
	        bm.recycle(); 		  //纹理加载成功后释放图片
		}
        return textures[0];
	}
	
	public static void releaseTexture(int textureId){
		if (textureId < 0) {
			return;
		}
		GLES30.glDeleteTextures(1, new int[]{textureId}, 0);
	}
	
	/**
	 * 生成视频纹理ID
	 * @author lixianke  @Date 2015-3-17 上午9:59:19
	 * @param
	 * @return
	 */
	@SuppressLint("InlinedApi")
	public static int createVideoTextureID(){
		int[] texture = new int[1];

		GLES30.glGenTextures(1, texture, 0);
		GLES30.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, texture[0]);
		GLES30.glTexParameterf(GLES11Ext.GL_TEXTURE_EXTERNAL_OES,
				GL10.GL_TEXTURE_MIN_FILTER,GL10.GL_LINEAR);        
		GLES30.glTexParameterf(GLES11Ext.GL_TEXTURE_EXTERNAL_OES,
				GL10.GL_TEXTURE_MAG_FILTER, GL10.GL_LINEAR);
		GLES30.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES,
				GL10.GL_TEXTURE_WRAP_S, GL10.GL_CLAMP_TO_EDGE);
		GLES30.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES,
				GL10.GL_TEXTURE_WRAP_T, GL10.GL_CLAMP_TO_EDGE);

		return texture[0];
	}
	
	/**
	 * 图片边框处理为透明
	 * @author lixianke  @Date 2015-3-27 下午2:38:43
	 * @param bm 原图bitmap对象
	 * @param isRecycle 原图是否回收
	 * @return 新创建的bitmp对象
	 */
	public static Bitmap handleBitmap(Bitmap bm, boolean isRecycle){
		int width = bm.getWidth();
		int height = bm.getHeight();
		
        Paint p = new Paint();
        Rect rect = new Rect(2, 2, width - 2, height - 2);
    	
    	Bitmap bitmap = Bitmap.createBitmap(width, height, Config.ARGB_8888);
        Canvas canvas = new Canvas(bitmap);
        canvas.drawColor(Color.TRANSPARENT);
        
        try {
        	canvas.drawBitmap(bm, rect, rect, p);
		} catch (Exception e) {
			return null;
		}
        
        
        if (isRecycle){
        	bm.recycle();
        }
        
        return bitmap;
	}
}
