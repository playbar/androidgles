package com.android.hook;

import android.os.Build;
import android.text.TextUtils;

import java.lang.reflect.Field;

/**
 * Created by houguoli on 2017/8/15.
 */

public class GLESHook {
//    static {
//        System.loadLibrary("glhook");
//    }
    public static native void printMapInfo();
    public static native void initHook();
    public static native void unInitHook();
    public static native void hookTest();

    public static void setsDaydreamPhoneOverrideForTesting() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
            try {
                Class cls = Class.forName("com.google.vr.ndk.base.DaydreamUtils");
                Field f = cls.getDeclaredField("sDaydreamPhoneOverrideForTesting");
                f.setAccessible(true);
                f.set(null, true);
            } catch (ClassNotFoundException e) {
                e.printStackTrace();
            } catch (NoSuchFieldException e) {
                e.printStackTrace();
            } catch (IllegalAccessException e) {
                e.printStackTrace();
            }
        }
    }


    public static void setsFingerprint() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            try {
                Class cls = Class.forName("android.os.Build");
                Field f = cls.getDeclaredField("FINGERPRINT");
                f.setAccessible(true);
                String fingerPrint = (String) f.get(null);
                if (!TextUtils.isEmpty(fingerPrint)) {
                    if (!fingerPrint.endsWith("dev-keys")) {
                        String end = fingerPrint.substring(fingerPrint.length() - 8);
                        String newFingerPrint = fingerPrint.replace(end, "dev-keys");
                        f.set(null, newFingerPrint);
                    }
                }
            } catch (ClassNotFoundException e) {
                e.printStackTrace();
            } catch (NoSuchFieldException e) {
                e.printStackTrace();
            } catch (IllegalAccessException e) {
                e.printStackTrace();
            }
        }
    }

}
