package com.orcchg.openglsample;

import android.content.Context;
import android.util.AttributeSet;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class RenderSurface extends SurfaceView implements SurfaceHolder.Callback {
  private static int mDesiredWidth = 512;   // in dp
  private static int mDesiredHeight = 512;  // in dp
  private int mWidth;
  private int mHeight;
  
  private NativeInterface nativeInterface;
  
  public RenderSurface(Context context) {
    this(context, null);
  }
  
  public RenderSurface(Context context, AttributeSet attrs) {
    this(context, attrs, 0);
  }
  
  public RenderSurface(Context context, AttributeSet attrs, int defStyleAttr) {
    super(context, attrs, defStyleAttr);
    getHolder().addCallback(this);
  }
  
  public void setNativeInterface(NativeInterface nativeInterface) {
    this.nativeInterface = nativeInterface;
  }

  @Override
  public void surfaceCreated(SurfaceHolder holder) {}

  @Override
  public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
    nativeInterface.setSurface(holder.getSurface());
  }

  @Override
  public void surfaceDestroyed(SurfaceHolder holder) {
    nativeInterface.setSurface(null);
  }
  
  @Override
  protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
    int widthMode = MeasureSpec.getMode(widthMeasureSpec);
    int widthSize = MeasureSpec.getSize(widthMeasureSpec);
    int heightMode = MeasureSpec.getMode(heightMeasureSpec);
    int heightSize = MeasureSpec.getSize(heightMeasureSpec);

    if (widthMode == MeasureSpec.EXACTLY) {
      mWidth = widthSize;
    } else if (widthMode == MeasureSpec.AT_MOST) {
      mWidth = Math.min(mDesiredWidth, widthSize);
    } else {
      mWidth = mDesiredWidth;
    }

    if (heightMode == MeasureSpec.EXACTLY) {
      mHeight = heightSize;
    } else if (heightMode == MeasureSpec.AT_MOST) {
      mHeight = Math.min(mDesiredHeight, heightSize);
    } else {
      mHeight = mDesiredHeight;
    }

    setMeasuredDimension(mWidth, mHeight);
  }
}
