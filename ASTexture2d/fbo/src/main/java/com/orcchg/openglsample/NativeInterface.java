package com.orcchg.openglsample;

import android.content.res.AssetManager;
import android.view.Surface;

public class NativeInterface {
  private final long descriptor;
  
  NativeInterface() {
    descriptor = init();
  }
  
  void prepareAssets(AssetManager assets, String internal_storage) { prepareAssets(descriptor, assets, internal_storage); }
  void setSurface(Surface surface) { setSurface(descriptor, surface); }
  void destroy() { destroy(descriptor); }
  
  private native long init();
  private native void prepareAssets(long descriptor, AssetManager assets, String internal_storage);
  private native void setSurface(long descriptor, Surface surface);
  private native void destroy(long descriptor);
}
