package com.baofeng.mojing.MojingTextureBaker.data;

/**
 * Created by zhangxingang on 2017/9/20.
 */

public class BitmapInfo {
    public BitmapInfo(int mTextureObjectId, int mWidth, int mHeight, int mX, int mY) {
        this.mTextureObjectId = mTextureObjectId;
        this.mWidth = mWidth;
        this.mHeight = mHeight;
        this.mX = mX;
        this.mY = mY;
    }

    public int mTextureObjectId;
    public int mWidth;
    public int mHeight;
    public int mX;
    public int mY;
}
