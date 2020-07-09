package com.bar.vkview;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLDisplay;

import com.bar.vkview.VKSurfaceView.EGLConfigChooser;

public abstract class BaseConfigChooser implements EGLConfigChooser {
    public BaseConfigChooser(int[] configSpec) {
        mConfigSpec = filterConfigSpec(configSpec);
//        mConfigSpec = s_configAttribs2;
    }

    private static int EGL_OPENGL_ES2_BIT = 4;
    private static int[] s_configAttribs2 =
            {
                    EGL10.EGL_RED_SIZE, 4,
                    EGL10.EGL_GREEN_SIZE, 4,
                    EGL10.EGL_BLUE_SIZE, 4,
                    EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                    EGL10.EGL_NONE
            };

    public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display) {
        int[] num_config = new int[1];
        if (!egl.eglChooseConfig(display, mConfigSpec, null, 0,
                num_config)) {
            throw new IllegalArgumentException("eglChooseConfig failed");
        }

        int numConfigs = num_config[0];

        if (numConfigs <= 0) {
            throw new IllegalArgumentException(
                    "No configs match configSpec");
        }

        EGLConfig[] configs = new EGLConfig[numConfigs];
        if (!egl.eglChooseConfig(display, mConfigSpec, configs, numConfigs,
                num_config)) {
            throw new IllegalArgumentException("eglChooseConfig#2 failed");
        }
        EGLConfig config = chooseConfig(egl, display, configs);
        if (config == null) {
            throw new IllegalArgumentException("No config chosen");
        }
        return config;
    }

    abstract EGLConfig chooseConfig(EGL10 egl, EGLDisplay display,
            EGLConfig[] configs);

    protected int[] mConfigSpec;

    private int[] filterConfigSpec(int[] configSpec) {
        if (VKSurfaceView.mEGLContextClientVersion != 2) {
            return configSpec;
        }
        /* We know none of the subclasses define EGL_RENDERABLE_TYPE.
         * And we know the configSpec is well formed.
         */
        int len = configSpec.length;
        int[] newConfigSpec = new int[len + 2];
        System.arraycopy(configSpec, 0, newConfigSpec, 0, len-1);
        newConfigSpec[len-1] = EGL10.EGL_RENDERABLE_TYPE;
        newConfigSpec[len] = 4; /* EGL_OPENGL_ES2_BIT */
        newConfigSpec[len+1] = EGL10.EGL_NONE;
        return newConfigSpec;
    }
}