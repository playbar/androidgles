package com.icebreaker.opengl;

import android.app.Activity;
import android.graphics.Bitmap;
import android.opengl.GLES20;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.widget.ImageView;

import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.IntBuffer;

import javax.microedition.khronos.opengles.GL10;

/**
 *
 */

public class MainActivity extends Activity {
    private TestRenderer glRenderer;
    private ImageView imageIv;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        SurfaceView sv = (SurfaceView)findViewById(R.id.sv_main_demo);
        imageIv = (ImageView)findViewById(R.id.iv_main_image);
        glRenderer = new TestRenderer();
        GLSurface glPbufferSurface = new GLSurface(512,512);
        glRenderer.addSurface(glPbufferSurface);
        glRenderer.startRender();
        glRenderer.requestRender();

        glRenderer.postRunnable(new Runnable() {
            @Override
            public void run() {
                IntBuffer ib = IntBuffer.allocate(512 * 512);
                GLES20.glReadPixels(0, 0, 512, 512, GL10.GL_RGBA, GL10.GL_UNSIGNED_BYTE, ib);

                final Bitmap bitmap = frameToBitmap(512, 512, ib);
                new Handler(Looper.getMainLooper()).post(new Runnable() {
                    @Override
                    public void run() {
                        imageIv.setImageBitmap(bitmap);
                    }
                });
            }
        });

        sv.getHolder().addCallback(new SurfaceHolder.Callback() {
            @Override
            public void surfaceCreated(SurfaceHolder surfaceHolder) {

            }

            @Override
            public void surfaceChanged(SurfaceHolder surfaceHolder, int format, int width, int height) {
                GLSurface glWindowSurface = new GLSurface(surfaceHolder.getSurface(),width,height);
                glRenderer.addSurface(glWindowSurface);
                glRenderer.requestRender();
            }

            @Override
            public void surfaceDestroyed(SurfaceHolder surfaceHolder) {

            }
        });
    }

    @Override
    protected void onDestroy() {
        glRenderer.release();
        glRenderer = null;
        super.onDestroy();
    }


    /**
     * 将数据转换成bitmap(OpenGL和Android的Bitmap色彩空间不一致，这里需要做转换)
     *
     * @param width 图像宽度
     * @param height 图像高度
     * @param ib 图像数据
     * @return bitmap
     */
    private static Bitmap frameToBitmap(int width, int height, IntBuffer ib) {
        int pixs[] = ib.array();
        // 扫描转置(OpenGl:左上->右下 Bitmap:左下->右上)
        for (int y = 0; y < height / 2; y++) {
            for (int x = 0; x < width; x++) {
                int pos1 = y * width + x;
                int pos2 = (height - 1 - y) * width + x;

                int tmp = pixs[pos1];
                pixs[pos1] = (pixs[pos2] & 0xFF00FF00) | ((pixs[pos2] >> 16) & 0xff) | ((pixs[pos2] << 16) & 0x00ff0000); // ABGR->ARGB
                pixs[pos2] = (tmp & 0xFF00FF00) | ((tmp >> 16) & 0xff) | ((tmp << 16) & 0x00ff0000);
            }
        }
        if (height % 2 == 1) { // 中间一行
            for (int x = 0; x < width; x++) {
                int pos = (height / 2 + 1) * width + x;
                pixs[pos] = (pixs[pos] & 0xFF00FF00) | ((pixs[pos] >> 16) & 0xff) | ((pixs[pos] << 16) & 0x00ff0000);
            }
        }

        return Bitmap.createBitmap(pixs, width, height, Bitmap.Config.ARGB_8888);
    }

    private String bitmapToJpeg(Bitmap bmp, String filePath) {
        try {
            FileOutputStream fos = new FileOutputStream(filePath);//注意app的sdcard读写权限问题
            bmp.compress(Bitmap.CompressFormat.JPEG, 80, fos);//压缩成png,100%显示效果
            fos.flush();
            fos.close();
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        }
        return filePath;
    }
}
