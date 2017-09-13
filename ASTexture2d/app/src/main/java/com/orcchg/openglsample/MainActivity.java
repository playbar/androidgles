package com.orcchg.openglsample;

import android.app.Activity;
import android.os.Bundle;

import com.openglesbook.R;


public class MainActivity extends Activity {
  static {
    System.loadLibrary("glesbook");
  }
  
  private NativeInterface ni;
  
  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_maingl);
    ni = new NativeInterface();
    ni.prepareAssets(getAssets(), getFilesDir().getAbsolutePath());
    RenderSurface surface = (RenderSurface) findViewById(R.id.surface_view);
    surface.setNativeInterface(ni);
  }
  
  @Override
  protected void onDestroy() {
    super.onDestroy();
    ni.destroy();
  }
}
