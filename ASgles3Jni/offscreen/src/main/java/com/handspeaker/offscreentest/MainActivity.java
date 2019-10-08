package com.handspeaker.offscreentest;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;

import com.test.offscreentest.R;

import android.support.v7.app.ActionBarActivity;
import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;
import android.opengl.GLES20;
import android.os.Bundle;
import android.os.Environment;
import android.view.Menu;
import android.view.MenuItem;

public class MainActivity extends ActionBarActivity {

    static 
    {
//    	System.loadLibrary("stlport_shared");
        System.loadLibrary("mygles");
    }
	
	public OffScreenRenderer mOffScreenRenderer;
	private IntBuffer RGBABuffer;
	//private ByteBuffer RGBABuffer;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		RGBABuffer = IntBuffer.allocate(512 * 512);
		//RGBABuffer = RGBABuffer.allocate(512 * 512*4);
		MyGles.release();
		MyGles.init();
		MyGles.draw();
		RGBABuffer.position(0);
		GLES20.glReadPixels(0, 0, 512, 512,GLES20.GL_RGBA,GLES20.GL_UNSIGNED_BYTE,RGBABuffer);
		//byte[] modelData=RGBABuffer.array();
		//byte[] ArData=new byte[modelData.length];
		int[] modelData=RGBABuffer.array();
		int[] ArData=new int[modelData.length];
		int offset1, offset2;
		for (int i = 0; i < 512; i++) {
			offset1 = i * 512;
			offset2 = (512 - i - 1) * 512;
			for (int j = 0; j < 512; j++) {
				int texturePixel = modelData[offset1 + j];
				int blue = (texturePixel >> 16) & 0xff;
				int red = (texturePixel << 16) & 0x00ff0000;
				int pixel = (texturePixel & 0xff00ff00) | red | blue;
				ArData[offset2 + j] = pixel;
			}
		}
		Bitmap modelBitmap = Bitmap.createBitmap(ArData,512,512,Bitmap.Config.ARGB_8888);
		saveBitmap(modelBitmap);
	}

	private void saveBitmap(Bitmap bitmap) {
		File file = new File(Environment.getExternalStorageDirectory().getAbsolutePath()
				+File.separator+"test_"+System.currentTimeMillis() + ".jpg");
		OutputStream file_out;
		try {
			file_out = new FileOutputStream(file);
			bitmap.compress(Bitmap.CompressFormat.JPEG, 100, file_out);
			file_out.flush();
			file_out.close();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}	
	
	@Override
	protected void onDestroy() {
		MyGles.release();
		super.onDestroy();
	}
	
}
