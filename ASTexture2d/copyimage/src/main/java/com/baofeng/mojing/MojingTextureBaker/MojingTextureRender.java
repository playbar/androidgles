package com.baofeng.mojing.MojingTextureBaker;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Typeface;
import android.opengl.GLSurfaceView;
import android.content.Context;

import com.baofeng.mojing.MojingTextureBaker.MojingTextureBaker;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import android.opengl.GLES30;
import android.opengl.GLUtils;

import java.io.InputStream;


public class MojingTextureRender implements GLSurfaceView.Renderer {
	int m_iBKColor;
	private Context mContext;
	private int mHandle;
	public MojingTextureRender(Context context)
	{
	mContext = context;
	}
	public void onDrawFrame(GL10 gl) {

			GLES30.glClearColor(m_iBKColor / 256.0f ,0,0,1);
			GLES30.glClear( GLES30.GL_DEPTH_BUFFER_BIT | GLES30.GL_COLOR_BUFFER_BIT);
			m_iBKColor = (m_iBKColor + 1)% 256;
        int handle = MojingTextureBaker.BeginBaker(256);
        MojingTextureBaker.AddTexture(handle,1, 16,32,16,32);
        MojingTextureBaker.AddTexture(handle,2, 16,32,80,90);
        MojingTextureBaker.AddTexture(handle,3, 16,32,180,230);
        MojingTextureBaker.AddTexture(handle,4, 16,32,100,20);
		//AddTexture(handle,5, 16,32,150,100);
        //AddTexture(handle,6, 16,32,20,50);
        //AddTexture(handle,7, 16,32,30,30);
        //AddTexture(handle,8, 16,32,40,50);
        //AddTexture(handle,9, 16,32,70,80);
        //AddTexture(handle,10, 16,32,60,20);
        MojingTextureBaker.RemoveTexture(handle,3, 16,32,180,230);
        String retResult = MojingTextureBaker.BakerTexture(handle, 100, true);
        MojingTextureBaker.EndBaker(0);
        MojingTextureBaker.OpenGLTest();
	}

	public void onSurfaceChanged(GL10 gl, int width, int height) {
		
	}

	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		mHandle = MojingTextureBaker.BeginBaker(512);
	}

	/* A native method that is implemented by the
     * 'MojingTextureBaker' native library, which is packaged
     * with this application.
     */


    /* this is used to load the 'MojingTextureBaker' library on application
     * startup. The library has already been unpacked into
     * /data/data/com.baofeng.mojing.MojingTextureBaker/lib/MojingTextureBaker.so at
     * installation time by the package manager.
     */
	 public int initImageTexture(int iResourceID) {

        InputStream is = mContext.getResources().openRawResource(iResourceID);//mContext.getResources().openRawResource(R.drawable.strip);
        //InputStream is = mContext.getResources().openRawResource(R.drawable.bmp123);
        Bitmap bitmap;
        bitmap = BitmapFactory.decodeStream(is);

        int[] textures = new int[1];
        GLES30.glGenTextures(1, textures, 0);

        int textureId = textures[0];
        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, textureId);

        GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MIN_FILTER, GLES30.GL_LINEAR);
        GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MAG_FILTER, GLES30.GL_LINEAR);
        GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_S, GLES30.GL_CLAMP_TO_EDGE);
        GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_T, GLES30.GL_CLAMP_TO_EDGE);


        GLUtils.texImage2D(GLES30.GL_TEXTURE_2D, 0, bitmap, 0);

		MojingTextureBaker.AddTexture(mHandle, 1, 0,0,bitmap.getWidth(), bitmap.getHeight());
        bitmap.recycle();

        return textureId;
    }
}