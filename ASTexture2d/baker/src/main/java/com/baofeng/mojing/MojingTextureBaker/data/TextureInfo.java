package com.baofeng.mojing.MojingTextureBaker.data;

import android.content.Context;

import com.baofeng.mojing.MojingTextureBaker.util.JasonHelper;
import com.google.gson.Gson;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

/**
 * Created by zhangxingang on 2017/9/21.
 */

public class TextureInfo {
    private static JasonHelper jasonHelper;
    public static List<JasonHelper.SubTexture> getSubTextureList(Context context, int resourceId) {
        Gson gson = new Gson();
        String jsonString;
        StringBuilder sb = new StringBuilder();
        InputStream in = context.getResources().openRawResource(resourceId);
        byte[] b = new byte[1024];
        int len = 0;
        try {
            while ((len = in.read(b)) != -1) {
                sb.append(new String(b, 0, len));
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        jsonString = sb.toString();
        jasonHelper = gson.fromJson(jsonString, JasonHelper.class);
        return jasonHelper.getSubTextureList();
    }

    public static List<JasonHelper.SubTexture> getSubTextureList(Context context, String json) {
        Gson gson = new Gson();
        jasonHelper = gson.fromJson(json, JasonHelper.class);
        return jasonHelper.getSubTextureList();
    }

    public  static int getBakedTextureSize()
    {
        return jasonHelper.getBakedTextureSize();
    }

//
//    public static List<BitmapInfo> getTextureInfoList(List<JasonHelper.SubTexture> texturesList){
//        List<BitmapInfo> bitmapInfos = new ArrayList<BitmapInfo>();
//        Iterator<BitmapInfo> it = bitmapInfos.iterator();
//        while(it.hasNext()){
//
//        }
//        return bitmapInfos;
//    }

}
