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

package com.bar.vkview;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.ViewGroup;
import android.widget.FrameLayout;

import javax.microedition.khronos.egl.EGLConfig;

public class VKViewActivity extends Activity {

    static {
        System.loadLibrary("vkutils");
    }

    VKViewTest mView;
    EGLConfig mEglConfig;


    @Override
    protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        FrameLayout layout = new FrameLayout( this );
        FrameLayout.LayoutParams params=new FrameLayout.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,   
                ViewGroup.LayoutParams.FILL_PARENT);//定义框架布局器参数  
        FrameLayout.LayoutParams tparams=new FrameLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,   
                ViewGroup.LayoutParams.WRAP_CONTENT);//定义显示组件参数  

        mView = new VKViewTest(getApplication());
        layout.addView(mView);
        setContentView(layout, params);
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
    }
    
    public boolean mbStop = true;
    
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        return super.onKeyDown(keyCode, event);
    }
    
    @Override
    public boolean onKeyLongPress(int keyCode, KeyEvent event) {
        return false;
    }
    
    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event){
        return super.onKeyUp(keyCode, event);
    }
    
    @Override
    public boolean onTouchEvent(MotionEvent event) {
    		Log.e("VKViewActivity", "onTouchEvent");
    		return super.onTouchEvent( event );
    }
}
