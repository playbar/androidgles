package com.baofeng.mojing.MojingTextureBaker.util;

import java.util.List;

/**
 * Created by zhangxingang on 2017/9/21.
 */

public class JasonHelper {
    int SubTextureCount;
    int BakedTextureSize;
    String BakedTextureID;
    List<SubTexture> BakedTexture;

    public int getSubTextureCount() {
        return SubTextureCount;
    }

    public void setSubTextureCount(int subTextureCount) {
        SubTextureCount = subTextureCount;
    }

    public int getBakedTextureSize() {
        return BakedTextureSize;
    }

    public void setBakedTextureSize(int bakedTextureSize) {
        BakedTextureSize = bakedTextureSize;
    }

    public String getBakedTextureID() {
        return BakedTextureID;
    }

    public void setBakedTextureID(String bakedTextureID) {
        BakedTextureID = bakedTextureID;
    }

    public List<SubTexture> getSubTextureList() {
        return BakedTexture;
    }

    public void setSubTextureList(List<SubTexture> subTextureList) {
        BakedTexture = subTextureList;
    }

    public class SubTexture{

        String SubTextureID;
        int[] Position;
        int[] Size;

        public String getSubTextureID() {
            return SubTextureID;
        }

        public void setSubTextureID(String subTextureID) {
            SubTextureID = subTextureID;
        }

        public int[] getPosition() {
            return Position;
        }

        public void setPosition(int[] position) {
            Position = position;
        }

        public int[] getSize() {
            return Size;
        }

        public void setSize(int[] size) {
            Size = size;
        }
    }

}
