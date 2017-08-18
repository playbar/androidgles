package com.android.gltest;

import java.io.IOException;
import java.io.InputStream;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;

import com.android.gles3jni.EglHelper;
import com.android.gles3jni.GLTextureUtils;
import com.android.gles3jni.GLES3JNIActivity.ExportTex;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Bitmap.Config;
import android.opengl.GLES30;
import android.opengl.GLSurfaceView.Renderer;
import android.opengl.GLUtils;
import android.opengl.Matrix;
import android.util.Log;
 
/**
 * @author gaofeng
 *
 */
public class GLRenderer implements Renderer {
     
       
    public static float[] projMatrix = new float[16];// 投影 
    public static float[] viewMatrix = new float[16];// 相机 
    public static float[] mViewPjMatrix;// 总变换矩阵 
    public static float[] matrixs = new float[16]; 
    public static int textureId = 0; 
    public static Context context; 
    MyDrawModel drawModel; 
    
    public static ThreadPoolExecutor executor = new ThreadPoolExecutor(1, 1, 200, TimeUnit.MILLISECONDS,
            new ArrayBlockingQueue<Runnable>(2000));
    
    public static ThreadPoolExecutor GetThreadPool(){
    		return executor;
    }
    
 
 
    public void setContext(Context context) {
        this.context = context;
    }
     
    public GLRenderer() {
    }
 
      @Override 
        public void onDrawFrame(GL10 arg0) { 
            GLES30.glClear( GLES30.GL_DEPTH_BUFFER_BIT | GLES30.GL_COLOR_BUFFER_BIT); 
//            Log.e("", "textureId:" + textureId); 
            if( textureId > 0 ){
            		drawModel.drawFrame(textureId); 
            }
        } 
       
        @Override 
        public void onSurfaceChanged(GL10 arg0, int w, int h) { 
            GLES30.glViewport(0, 0, w, h); 
            float ratio = (float) w / h; 
            Matrix.frustumM(projMatrix, 0, -ratio, ratio, -1, 1, 1, 10);//投影矩阵设置 
            Matrix.setLookAtM(viewMatrix, 0, 0, 0, 3, 0, 0, 0, 0.0f, 1.0f, 0.0f);//摄像机坐标设置 
            
        } 
      
        @Override 
        public void onSurfaceCreated(GL10 g, EGLConfig eglConfig) { 
            GLES30.glClearColor(0.5f,0.5f,0.5f, 1.0f);   
            GLES30.glEnable(GLES30.GL_DEPTH_TEST); 
            EGL10 egl = (EGL10)EGLContext.getEGL();
            int[] attrib_list = {0x3098, 3, EGL10.EGL_NONE };
//            EGLDisplay display = egl.eglGetCurrentDisplay();
            EGLContext eglContext = egl.eglGetCurrentContext();
            GenTexTask.mEglDisplay = egl.eglGetCurrentDisplay();
//            GenTexTask.mEglConfig = eglConfig;
            GenTexTask.mEglContext = egl.eglCreateContext(GenTexTask.mEglDisplay, eglConfig, eglContext, attrib_list); 
           
            
            drawModel = new MyDrawModel(); 
            drawModel.init(); 
           
            GLES30.glDisable(GLES30.GL_CULL_FACE); 
            textureId = createTexture("");
//            testtt();
        }
        
        public static void testtt(){
        		InputStream ins = null;
			try {
				ins = context.getAssets().open("test.jpg");
				Bitmap bmp = BitmapFactory.decodeStream(ins); 
        			GenTexTask myta = new GenTexTask(123, bmp);
        			GLRenderer.GetThreadPool().execute(myta);
			} catch (IOException e) {
				e.printStackTrace();
			} finally {
				try {
					if (ins != null) {
						ins.close();
					}
				} catch (IOException e) {
					e.printStackTrace();
				}
			}//end try
        }
        
        public static int createTexture( String strPath ){
        		int texId = 0;
        		InputStream ins = null;
			try {
				ins = context.getAssets().open("test.jpg");
				Bitmap bmp = BitmapFactory.decodeStream(ins); 
				texId = createTexture( bmp );
			} catch (IOException e) {
				e.printStackTrace();
			} finally {
				try {
					if (ins != null) {
						ins.close();
					}
				} catch (IOException e) {
					e.printStackTrace();
				}
			}//end try
			return texId;
        }
     
        public static int createTexture(Bitmap bitmap) { 
            int[] textures = new int[1]; 
            GLES30.glGenTextures(1, textures, 0);
            int textureId = textures[0]; 
            GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, textureId); 
            GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MIN_FILTER,GLES30.GL_NEAREST);  
            GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D,GLES30.GL_TEXTURE_MAG_FILTER,GLES30.GL_LINEAR); 
            GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_S,GLES30.GL_CLAMP_TO_EDGE); 
            GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_T,GLES30.GL_CLAMP_TO_EDGE); 
            //上面是纹理贴图的取样方式，包括拉伸方式，取临近值和线性值 
            GLUtils.texImage2D(GLES30.GL_TEXTURE_2D, 0, bitmap, 0);//让图片和纹理关联起来，加载到OpenGl空间中 
            Log.d("OPENGL","bitmap:" + bitmap); 
//            bitmap.recycle();//不需要，可以释放 
            return textureId; 
        } 
        
        public static int createTexture(InputStream ins) { 
            int[] textures = new int[1]; 
            GLES30.glGenTextures(1, textures, 0);
            int textureId = textures[0]; 
            GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, textureId); 
            GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MIN_FILTER,GLES30.GL_NEAREST);  
            GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D,GLES30.GL_TEXTURE_MAG_FILTER,GLES30.GL_LINEAR); 
            GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_S,GLES30.GL_CLAMP_TO_EDGE); 
            GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_T,GLES30.GL_CLAMP_TO_EDGE); 
            //上面是纹理贴图的取样方式，包括拉伸方式，取临近值和线性值 
            Bitmap bitmap = BitmapFactory.decodeStream(ins); 
            GLUtils.texImage2D(GLES30.GL_TEXTURE_2D, 0, bitmap, 0);//让图片和纹理关联起来，加载到OpenGl空间中 
            Log.d("OPENGL","bitmap:" + bitmap); 
            bitmap.recycle();//不需要，可以释放 
            return textureId; 
        } 
       
}
