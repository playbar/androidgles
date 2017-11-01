package com.baofeng.mojing.MojingTextureBaker.util;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLES30;
import android.opengl.GLUtils;
import android.util.Log;

import com.baofeng.mojing.MojingTextureBaker.data.BitmapInfo;

/**
 * Created by zhangxingang on 2017/9/15.
 */

public class TextureHelper {
    private static final String TAG = "TextureHelper";

	private static int index = 0;

    /**
     * Loads a texture from a resource ID, returning the OpenGL ID for that
     * texture. Returns 0 if the load failed.
     *
     * @param context
     * @param resourceId
     * @return
     */
    public static int loadTexture(Context context, int resourceId) {
        final int textureId[] = new int[1];
        GLES30.glGenTextures(1, textureId, 0);
        if (0 == textureId[0]) {
            if (LoggerConfig.ON) {
                Log.i(TAG, "Could not generate a new OpenGL texture object.");
            }
            return 0;
        }
        final BitmapFactory.Options options = new BitmapFactory.Options();
        options.inScaled = false;

        // Read in the resource
        final Bitmap bitmap = BitmapFactory.decodeResource(
                context.getResources(), resourceId, options);

        if (bitmap == null) {
            if (LoggerConfig.ON) {
                Log.w(TAG, "Resource ID " + resourceId
                        + " could not be decoded.");
            }
            GLES30.glDeleteTextures(1, textureId, 0);
            return 0;
        }
        // Bind to the texture in OpenGL
        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, textureId[0]);

        // Set filtering: a default must be set, or the texture will be
        // black.
        GLES30.glTexParameteri(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MIN_FILTER, GLES30.GL_LINEAR_MIPMAP_LINEAR);
        GLES30.glTexParameteri(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MAG_FILTER, GLES30.GL_LINEAR);
        GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_S, GLES30.GL_CLAMP_TO_EDGE);//纹理边缘延伸效果
        GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_T, GLES30.GL_CLAMP_TO_EDGE);
        // Load the bitmap into the bound texture.
        GLUtils.texImage2D(GLES30.GL_TEXTURE_2D, 0, bitmap, 0);

        // Note: Following code may cause an error to be reported in the
        // ADB log as follows: E/IMGSRV(20095): :0: HardwareMipGen:
        // Failed to generate texture mipmap levels (error=3)
        // No OpenGL error will be encountered (glGetError() will return
        // 0). If this happens, just squash the source image to be
        // square. It will look the same because of texture coordinates,
        // and mipmap generation will work.
        GLES30.glGenerateMipmap(GLES30.GL_TEXTURE_2D);
        // Recycle the bitmap, since its data has been loaded into
        // OpenGL.
        bitmap.recycle();

        // Unbind from the texture.
        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, 0);
        return textureId[0];
    }

    /**
     * Loads a texture from a resource ID, returning the OpenGL ID for that
     * texture. Returns 0 if the load failed.
     *
     * @param context
     * @param resourceId
     * @return BitmapInfo
     */
    public static BitmapInfo loadTextureOutBitmapInfo(Context context, int resourceId) {
        int width = 0, height = 0;
        final int textureId[] = new int[1];
        GLES30.glGenTextures(1, textureId, 0);
        if (0 == textureId[0]) {
            if (LoggerConfig.ON) {
                Log.i(TAG, "Could not generate a new OpenGL texture object.");
            }
            return null;
        }
        final BitmapFactory.Options options = new BitmapFactory.Options();
        options.inScaled = false;

        // Read in the resource
        final Bitmap bitmap = BitmapFactory.decodeResource(
                context.getResources(), resourceId, options);

        if (bitmap == null) {
            if (LoggerConfig.ON) {
                Log.w(TAG, "Resource ID " + resourceId
                        + " could not be decoded.");
            }
            GLES30.glDeleteTextures(1, textureId, 0);
            return null;
        }
        // Bind to the texture in OpenGL
        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, textureId[0]);

        // Set filtering: a default must be set, or the texture will be
        // black.
        GLES30.glTexParameteri(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MIN_FILTER, GLES30.GL_LINEAR_MIPMAP_LINEAR);
        GLES30.glTexParameteri(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MAG_FILTER, GLES30.GL_LINEAR);
        GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_S, GLES30.GL_CLAMP_TO_EDGE);//纹理边缘延伸效果
        GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_T, GLES30.GL_CLAMP_TO_EDGE);
        // Load the bitmap into the bound texture.
        GLUtils.texImage2D(GLES30.GL_TEXTURE_2D, 0, bitmap, 0);

        // Note: Following code may cause an error to be reported in the
        // ADB log as follows: E/IMGSRV(20095): :0: HardwareMipGen:
        // Failed to generate texture mipmap levels (error=3)
        // No OpenGL error will be encountered (glGetError() will return
        // 0). If this happens, just squash the source image to be
        // square. It will look the same because of texture coordinates,
        // and mipmap generation will work.
        GLES30.glGenerateMipmap(GLES30.GL_TEXTURE_2D);
        // Recycle the bitmap, since its data has been loaded into
        // OpenGL.
        width = bitmap.getWidth();
        height = bitmap.getHeight();
        bitmap.recycle();

        // Unbind from the texture.
        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, 0);
        return new BitmapInfo(textureId[0], index++, width, height, 0, 0);
    }

    public static int loadCubemap(Context context, int[] cubeResources, boolean isBitmapLoaded) {
        final int textureId[] = new int[1];
        GLES30.glGenTextures(1, textureId, 0);
        if (0 == textureId[0]) {
            if (LoggerConfig.ON) {
                Log.i(TAG, "Could not generate a new OpenGL texture object.");
            }
            return 0;
        }
        if (isBitmapLoaded) {
            return 0;
        } else {
            final BitmapFactory.Options options = new BitmapFactory.Options();
            options.inScaled = false;
            final Bitmap[] cubeBitmaps = new Bitmap[6];
            for (int i = 0; i < 6; i++) {
                cubeBitmaps[i] =
                        BitmapFactory.decodeResource(context.getResources(),
                                cubeResources[i], options);

                if (cubeBitmaps[i] == null) {
                    if (LoggerConfig.ON) {
                        Log.w(TAG, "Resource ID " + cubeResources[i]
                                + " could not be decoded.");
                    }
                    GLES30.glDeleteTextures(1, textureId, 0);
                    return 0;
                }
            }
            // Linear filtering for minification and magnification
            GLES30.glBindTexture(GLES30.GL_TEXTURE_CUBE_MAP, textureId[0]);

            GLES30.glTexParameteri(GLES30.GL_TEXTURE_CUBE_MAP, GLES30.GL_TEXTURE_MIN_FILTER, GLES30.GL_LINEAR);
            GLES30.glTexParameteri(GLES30.GL_TEXTURE_CUBE_MAP, GLES30.GL_TEXTURE_MAG_FILTER, GLES30.GL_LINEAR);
            GLUtils.texImage2D(GLES30.GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, cubeBitmaps[0], 0);
            GLUtils.texImage2D(GLES30.GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, cubeBitmaps[1], 0);

            GLUtils.texImage2D(GLES30.GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, cubeBitmaps[2], 0);
            GLUtils.texImage2D(GLES30.GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, cubeBitmaps[3], 0);

            GLUtils.texImage2D(GLES30.GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, cubeBitmaps[4], 0);
            GLUtils.texImage2D(GLES30.GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, cubeBitmaps[5], 0);
            GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, 0);
            for (Bitmap bitmap : cubeBitmaps) {
                bitmap.recycle();
            }
        }
        return textureId[0];
    }

    public static int createCubemap(int rect) {
        final int textureId[] = new int[1];
        GLES30.glGenTextures(1, textureId, 0);
        if (0 == textureId[0]) {
            if (LoggerConfig.ON) {
                Log.i(TAG, "Could not generate a new OpenGL texture object.");
            }
            return 0;
        }
        GLES30.glBindTexture(GLES30.GL_TEXTURE_CUBE_MAP, textureId[0]);
        GLES30.glTexParameteri(GLES30.GL_TEXTURE_CUBE_MAP, GLES30.GL_TEXTURE_MIN_FILTER, GLES30.GL_LINEAR);
        GLES30.glTexParameteri(GLES30.GL_TEXTURE_CUBE_MAP, GLES30.GL_TEXTURE_MAG_FILTER, GLES30.GL_LINEAR);
        GLES30.glTexParameterf(GLES30.GL_TEXTURE_CUBE_MAP, GLES30.GL_TEXTURE_WRAP_S, GLES30.GL_CLAMP_TO_EDGE);//纹理边缘延伸效果
        GLES30.glTexParameterf(GLES30.GL_TEXTURE_CUBE_MAP, GLES30.GL_TEXTURE_WRAP_T, GLES30.GL_CLAMP_TO_EDGE);
        GLES30.glTexImage2D(GLES30.GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GLES30.GL_RGBA, rect, rect, 0, GLES30.GL_RGBA, GLES30.GL_UNSIGNED_BYTE, null);
        GLES30.glTexImage2D(GLES30.GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GLES30.GL_RGBA, rect, rect, 0, GLES30.GL_RGBA, GLES30.GL_UNSIGNED_BYTE, null);
        GLES30.glTexImage2D(GLES30.GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GLES30.GL_RGBA, rect, rect, 0, GLES30.GL_RGBA, GLES30.GL_UNSIGNED_BYTE, null);
        GLES30.glTexImage2D(GLES30.GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GLES30.GL_RGBA, rect, rect, 0, GLES30.GL_RGBA, GLES30.GL_UNSIGNED_BYTE, null);
        GLES30.glTexImage2D(GLES30.GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GLES30.GL_RGBA, rect, rect, 0, GLES30.GL_RGBA, GLES30.GL_UNSIGNED_BYTE, null);
        GLES30.glTexImage2D(GLES30.GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GLES30.GL_RGBA, rect, rect, 0, GLES30.GL_RGBA, GLES30.GL_UNSIGNED_BYTE, null);
        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, 0);
        return textureId[0];
    }

    public static int loadCubemapFromTexture(int cubemapTexture, int[] textureResources, int rect) {
        for (int i = 0; i < 6; i++) {
            int framebuffer[] = new int[1];
            GLES30.glGenFramebuffers(1, framebuffer, 0);
            if (0 == framebuffer[0]) {
                if (LoggerConfig.ON) {
                    Log.i(TAG, "Could not generate a new OpenGL framebuffer object.");
                }
                return 0;
            }
            GLES30.glBindFramebuffer(GLES30.GL_READ_FRAMEBUFFER, framebuffer[0]);
            GLES30.glBlendFunc(GLES30.GL_SRC_ALPHA, GLES30.GL_ONE_MINUS_SRC_ALPHA);
            GLES30.glEnable(GLES30.GL_BLEND);
            GLES30.glFramebufferTexture2D(GLES30.GL_READ_FRAMEBUFFER, GLES30.GL_COLOR_ATTACHMENT0,
                    GLES30.GL_TEXTURE_2D, textureResources[i], 0);
            int error = GLES30.glGetError();
            GLES30.glBindTexture(GLES30.GL_TEXTURE_CUBE_MAP, cubemapTexture);
            error = GLES30.glGetError();
            GLES30.glCopyTexSubImage2D(GLES30.GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, 0, 0, 0, rect, rect);
            error = GLES30.glGetError();
            Log.e(TAG, "loadCubemapFromTexture: glGetError=" + error);
            GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, 0);
            GLES30.glBindFramebuffer(GLES30.GL_FRAMEBUFFER, 0);
            GLES30.glDeleteFramebuffers(1, framebuffer, 0);
        }
        return cubemapTexture;
    }
    public static int loadCubemapFromTexture(int cubemapTexture, BitmapInfo[] bitmapInfos) {
        for (int i = 0; i < 6; i++) {
            int framebuffer[] = new int[1];
            GLES30.glGenFramebuffers(1, framebuffer, 0);
            if (0 == framebuffer[0]) {
                if (LoggerConfig.ON) {
                    Log.i(TAG, "Could not generate a new OpenGL framebuffer object.");
                }
                return 0;
            }
            GLES30.glBindFramebuffer(GLES30.GL_READ_FRAMEBUFFER, framebuffer[0]);
            GLES30.glBlendFunc(GLES30.GL_SRC_ALPHA, GLES30.GL_ONE_MINUS_SRC_ALPHA);
            GLES30.glEnable(GLES30.GL_BLEND);
            GLES30.glFramebufferTexture2D(GLES30.GL_READ_FRAMEBUFFER, GLES30.GL_COLOR_ATTACHMENT0,
                    GLES30.GL_TEXTURE_2D, bitmapInfos[i].mTextureObjectId, 0);
            int error = GLES30.glGetError();
            GLES30.glBindTexture(GLES30.GL_TEXTURE_CUBE_MAP, cubemapTexture);
            error = GLES30.glGetError();
            GLES30.glCopyTexSubImage2D(GLES30.GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, 0, 0, 0, bitmapInfos[i].mWidth, bitmapInfos[i].mHeight);
            error = GLES30.glGetError();
            Log.e(TAG, "loadCubemapFromTexture: glGetError=" + error);
            GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, 0);
            GLES30.glBindFramebuffer(GLES30.GL_FRAMEBUFFER, 0);
            GLES30.glDeleteFramebuffers(1, framebuffer, 0);
        }
        return cubemapTexture;
    }
}
