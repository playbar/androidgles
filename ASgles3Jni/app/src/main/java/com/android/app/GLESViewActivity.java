/*
 * Copyright 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.app;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.ViewGroup;
import android.widget.FrameLayout;

import com.android.gles3jni.GLES3View;
import com.android.gltest.GLRenderer;
import com.android.gltest.GenTexTask;

import java.io.IOException;
import java.io.InputStream;
import java.util.Timer;
import java.util.TimerTask;

import javax.microedition.khronos.egl.EGLConfig;

public class GLESViewActivity extends Activity {

    GLES3View mView;
    EGLConfig mEglConfig;
    
    public class ExportTex implements GenTexTask.ExportTextureId{

 		@Override
 		public void exportId(int texid, int hashcode) {
 			// TODO Auto-generated method stub
 			GLRenderer.textureId = texid;
 		}
     	
     } 

    public final TimerTask task = new TimerTask(){   
    	
        public void run(){  
			InputStream ins = null;
			try {
				ins = getAssets().open("test.jpg");
				Bitmap bmp = BitmapFactory.decodeStream(ins); 
				ExportTex ex = new ExportTex();
        			GenTexTask myta = new GenTexTask(123, bmp);
        			myta.SetInterfaceTex( ex );
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
    };   
    
    public void TestMakeCurrent(){
  	  	Timer timer = new Timer(); 
  	  	timer.schedule(task, 4000 ); 
    }


    @Override protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        FrameLayout layout = new FrameLayout( this );
        FrameLayout.LayoutParams params=new FrameLayout.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,   
                ViewGroup.LayoutParams.FILL_PARENT);//定义框架布局器参数  
        FrameLayout.LayoutParams tparams=new FrameLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,   
                ViewGroup.LayoutParams.WRAP_CONTENT);//定义显示组件参数  
//        TextView txtview = new TextView(this);//定义组件  
//        txtview.setText("欢迎使用框架布局");  
//        EditText et = new EditText(this);  
//        et.setText("请选择你喜欢的布局");  
//        Button but=new Button(this);  
//        but.setText("按我");  
//        layout.addView(txtview, tparams);//添加组件  
//        layout.addView(et, tparams);  
//        layout.addView(but, tparams);  
        
   
        mView = new GLES3View(getApplication());
       
//        mView.setZOrderOnTop(true);
//        mView.setEGLConfigChooser(8, 8, 8, 8, 16, 0);
//        mView.getHolder().setFormat(PixelFormat.TRANSLUCENT);
        layout.addView(mView);
//        layout.addView(mView2);
//        mView.onPause();
//		mView.setVisibility(View.GONE);
        setContentView(layout, params);
        
//        Timer timer = new Timer(); 
//        timer.schedule(task, 5000 ); 
       
//        MojingSDK.Init(this, false );
    }

    @Override 
    protected void onPause() {
        super.onPause();
        mView.onPause();
    }

    @Override 
    protected void onResume() {
        super.onResume();
        mView.onResume();
        
//        TestMakeCurrent();
    		
//        EGL10 egl = (EGL10)EGLContext.getEGL();
//        EGLDisplay eglDisplay = egl.eglGetCurrentDisplay();
//  	  	EGLSurface eglSurface = egl.eglGetCurrentSurface(EGL10.EGL_DRAW);
////  	  	EGLConfig eglConfig 
//  	  	EGLContext eglContext = egl.eglGetCurrentContext();
    }
    
    public boolean mbStop = true;
    
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
//        if( keyCode == KeyEvent.KEYCODE_BACK ){
////    	        BuildTexThread thread = new BuildTexThread( this );
////    	        thread.init(mView.getEglConfig());
////    	        thread.start();
//            return false;
//        }else{
//            return super.onKeyDown(keyCode, event);
//        }
        return super.onKeyDown(keyCode, event);
    }
    
    @Override
    public boolean onKeyLongPress(int keyCode, KeyEvent event) {
        return false;
    }
    
    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event){
//        if( keyCode == KeyEvent.KEYCODE_BACK ){
//            Log.e("GLESJNIActivity", "onKeyUp");
////    			mView.switchThread(mbStop);
//            mbStop = !mbStop;
//
//            return false;
//        }else{
//            return super.onKeyUp(keyCode, event);
//        }
        return super.onKeyUp(keyCode, event);
    }
    
    @Override
    public boolean onTouchEvent(MotionEvent event) {
    		Log.e("GLESJNIActivity", "onTouchEvent");	
//    		mView.onTouchEvent( event );
    		return super.onTouchEvent( event );
    }
}
