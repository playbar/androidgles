package com.baofeng.mojing.MojingTextureBaker;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.opengl.GLES30;
import android.opengl.GLSurfaceView;
import android.opengl.GLUtils;
import android.util.Log;

import com.baofeng.mojing.MojingTextureBaker.data.BitmapHelper;
import com.baofeng.mojing.MojingTextureBaker.data.BitmapInfo;
import com.baofeng.mojing.MojingTextureBaker.data.TextureInfo;
import com.baofeng.mojing.MojingTextureBaker.program.BakerShaderProgram;
import com.baofeng.mojing.MojingTextureBaker.program.SkyboxShaderProgram;
import com.baofeng.mojing.MojingTextureBaker.util.JasonHelper;
import com.baofeng.mojing.MojingTextureBaker.util.TextureHelper;

import java.io.InputStream;
import java.util.List;
import java.util.Vector;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import static android.opengl.Matrix.multiplyMM;
import static android.opengl.Matrix.rotateM;
import static android.opengl.Matrix.setIdentityM;


/**
 * Created by zhangxingang on 2017/9/14.
 */

public class MojingGLCubemapRender implements GLSurfaceView.Renderer {
    private static final String TAG = "MojingGLCubemapRender";
    private Context mContext;
    private SkyboxShaderProgram mSkyboxShaderProgram;
    private BakerShaderProgram mBakerShaderProgram;
    private List<JasonHelper.SubTexture> mSubTexturesList;
    private int mMojingTexture;
    private int mSkyboxTexture;
    private int mTextureBakerHandle;
    private int mTextureBaker;
    private int mCubemapFaceTextureIndex;
    private int mCubemapFrameBuffer = 0;
    private int[] mBakerTexture;
    private final float[] mViewMatrix = new float[16];
    private final float[] mProjectionMatrix = new float[16];
    private final float[] mViewProjectionMatrix = new float[16];

    private float xRotation;
    private float yRotation;

    private int mWidth;
    private int mHeight;


    public MojingGLCubemapRender(Context mContext) {
        this.mContext = mContext;
        mCubemapFaceTextureIndex = 0;
        mSubTexturesList = TextureInfo.getSubTextureList(mContext, R.raw.mojingtexturebaker);
    }

    @Override
    public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
        GLES30.glClearColor(0.7f, 0.7f, 0.7f, 0.7f);
        mMojingTexture = TextureHelper.loadTexture(mContext, R.drawable.mojingtexturebaker);
        //int ii[] = new int[1];
        //GLES30.glGetIntegerv(GLES30.GL_MAX_DRAW_BUFFERS, ii, 0);
        mSkyboxShaderProgram = new SkyboxShaderProgram(mContext);
        mBakerShaderProgram = new BakerShaderProgram(mContext);
        BitmapInfo mBitmapInfo[] = BitmapHelper.getBitmapTexture(mContext);
        mSkyboxTexture = TextureHelper.createCubemap(1024);
        mBakerTexture = new int[9];
        TextureHelper.loadCubemapFromTexture(mSkyboxTexture, mBitmapInfo);
        mBakerShaderProgram.initCubmapFaceVertexIndex();
        mBakerShaderProgram.initCubmapFaceVertex();
    }

    @Override
    public void onSurfaceChanged(GL10 gl10, int width, int height) {
        mWidth = width;
        mHeight = height;
        GLES30.glViewport(0, 0, width, height);
        //android.opengl.Matrix.setLookAtM(mProjectionMatrix ,0,0,0,0,0,0,-1f,0,1,0);
        android.opengl.Matrix.perspectiveM(mProjectionMatrix, 0, 45, (float) width / height, 0.1f, 100f);
        android.opengl.Matrix.translateM(mProjectionMatrix, 0, 0f, 0f, -5f);
//        MatrixHelper.perspectiveM(projectionMatrix, 45, (float) width
//                / (float) height, 1f, 10f);
    }


    private  int m_iFrameCount = 0;
    @Override
    public void onDrawFrame(GL10 gl10) {
        GLES30.glClear(GLES30.GL_COLOR_BUFFER_BIT);
        final int iChangeTime = 1;
        if (m_iFrameCount  % (iChangeTime * 60) == 0) {
            drawCubeFaceHalf1();
            drawCubeFaceHalf2();
        }
        drawSkybox();
        m_iFrameCount++;
    }

    private void drawCubeFaceHalf2() {
        mCubemapFaceTextureIndex = mBakerShaderProgram.initCubmapFaceFromTextureInfo(mSubTexturesList, mCubemapFaceTextureIndex);
        GLES30.glViewport(0, 0, 1024, 1024);
        if (mCubemapFrameBuffer == 0)
        {
            int []iTemp = new int[1];
            GLES30.glGenFramebuffers(1,iTemp,0);
            mCubemapFrameBuffer = iTemp[0];
        }
        GLES30.glBindFramebuffer(GLES30.GL_FRAMEBUFFER , mCubemapFrameBuffer);
        GLES30.glFramebufferTexture2D(GLES30.GL_FRAMEBUFFER, GLES30.GL_COLOR_ATTACHMENT0, GLES30.GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                mSkyboxTexture, 0);
        GLES30.glFramebufferTexture2D(GLES30.GL_FRAMEBUFFER, GLES30.GL_COLOR_ATTACHMENT1, GLES30.GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
                mSkyboxTexture, 0);
        GLES30.glFramebufferTexture2D(GLES30.GL_FRAMEBUFFER, GLES30.GL_COLOR_ATTACHMENT2, GLES30.GL_TEXTURE_CUBE_MAP_POSITIVE_X,
                mSkyboxTexture, 0);
        int attachments[] = {
                GLES30.GL_COLOR_ATTACHMENT0,
                GLES30.GL_COLOR_ATTACHMENT1,
                GLES30.GL_COLOR_ATTACHMENT2,
        };
        GLES30.glDrawBuffers(4, attachments, 0);

        setIdentityM(mViewMatrix, 0);
        rotateM(mViewMatrix, 0, 180, 0f, 0f, 1f);

        mBakerShaderProgram.useProgram();
        mBakerShaderProgram.setUniforms(mViewMatrix, mMojingTexture);
        //mBakerShaderProgram.draw(mBakerTexture);

        GLES30.glBindFramebuffer(GLES30.GL_FRAMEBUFFER , 0);
    }

    private void drawCubeFaceHalf1()
    {
        mCubemapFaceTextureIndex = mBakerShaderProgram.initCubmapFaceFromTextureInfo(mSubTexturesList, 0);
        GLES30.glViewport(0, 0, 1024, 1024);
        if (mCubemapFrameBuffer == 0)
        {
            int []iTemp = new int[1];
            GLES30.glGenFramebuffers(1,iTemp,0);
            mCubemapFrameBuffer = iTemp[0];
        }
        GLES30.glBindFramebuffer(GLES30.GL_FRAMEBUFFER , mCubemapFrameBuffer);
        GLES30.glFramebufferTexture2D(GLES30.GL_FRAMEBUFFER, GLES30.GL_COLOR_ATTACHMENT0, GLES30.GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
                mSkyboxTexture, 0);
        GLES30.glFramebufferTexture2D(GLES30.GL_FRAMEBUFFER, GLES30.GL_COLOR_ATTACHMENT1, GLES30.GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
            mSkyboxTexture, 0);
        GLES30.glFramebufferTexture2D(GLES30.GL_FRAMEBUFFER, GLES30.GL_COLOR_ATTACHMENT2, GLES30.GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                mSkyboxTexture, 0);
        int attachments[] = {
                GLES30.GL_COLOR_ATTACHMENT0,
                GLES30.GL_COLOR_ATTACHMENT1,
                GLES30.GL_COLOR_ATTACHMENT2,
        };
        GLES30.glDrawBuffers(4, attachments, 0);

        setIdentityM(mViewMatrix, 0);
        rotateM(mViewMatrix, 0, 180, 0f, 0f, 1f);

        mBakerShaderProgram.useProgram();
        mBakerShaderProgram.setUniforms(mViewMatrix, mMojingTexture);
        //mBakerShaderProgram.draw(mBakerTexture);

        GLES30.glBindFramebuffer(GLES30.GL_FRAMEBUFFER , 0);
    }

    private void drawSkybox() {
        GLES30.glViewport(0, 0, mWidth, mHeight);
        setIdentityM(mViewMatrix, 0);
        rotateM(mViewMatrix, 0, -yRotation, 1f, 0f, 0f);
        rotateM(mViewMatrix, 0, -xRotation, 0f, 1f, 0f);
        multiplyMM(mViewProjectionMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
        mSkyboxShaderProgram.useProgram();
        mSkyboxShaderProgram.setUniforms(mViewProjectionMatrix, mSkyboxTexture);
        mSkyboxShaderProgram.draw();
//        mBakerShaderProgram.useProgram();
//        mBakerShaderProgram.setUniforms(mViewProjectionMatrix, mBakerTexture);
//        mBakerShaderProgram.draw(mBakerTexture);
    }

    private void initImageForCubemap(int[] mCubemapId) {
        int framebuffer[] = new int[1];
        GLES30.glGenFramebuffers(1, framebuffer, 0);
        GLES30.glBindFramebuffer(GLES30.GL_READ_FRAMEBUFFER, framebuffer[0]);


        GLES30.glBlendFunc(GLES30.GL_SRC_ALPHA, GLES30.GL_ONE_MINUS_SRC_ALPHA);
        GLES30.glEnable(GLES30.GL_BLEND);
        GLES30.glFramebufferTexture2D(GLES30.GL_READ_FRAMEBUFFER, GLES30.GL_COLOR_ATTACHMENT0,
                GLES30.GL_TEXTURE_CUBE_MAP, 2, 0);
        GLES30.glBindTexture(GLES30.GL_TEXTURE_CUBE_MAP, mCubemapId[0]);
        GLES30.glCopyTexSubImage2D(GLES30.GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, 0, 0, 0, 0, 100, 100);
        GLES30.glCopyTexSubImage2D(GLES30.GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 0, 0, 0, 0, 100, 100);
        GLES30.glCopyTexSubImage2D(GLES30.GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, 0, 0, 0, 0, 100, 100);
        GLES30.glCopyTexSubImage2D(GLES30.GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, 0, 0, 0, 0, 100, 100);
        GLES30.glCopyTexSubImage2D(GLES30.GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, 0, 0, 0, 0, 100, 100);
        GLES30.glCopyTexSubImage2D(GLES30.GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, 0, 0, 0, 0, 100, 100);

        GLES30.glBindTexture(GLES30.GL_TEXTURE_CUBE_MAP, 0);
    }

    private int initImageTexture(int handle, int iResourceID, boolean flag) {

        InputStream is = mContext.getResources().openRawResource(iResourceID);//mContext.getResources().openRawResource(R.drawable.strip);
        //InputStream is = mContext.getResources().openRawResource(R.drawable.bmp123);
        Bitmap bitmap;
        bitmap = BitmapFactory.decodeStream(is);

        int[] textures = new int[1];
        GLES30.glGenTextures(1, textures, 0);

        int textureId = textures[0];
        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, textureId);

        GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MIN_FILTER, GLES30.GL_LINEAR);//线性过滤差值
        GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MAG_FILTER, GLES30.GL_LINEAR);
        GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_S, GLES30.GL_CLAMP_TO_EDGE);//纹理边缘延伸效果
        GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_T, GLES30.GL_CLAMP_TO_EDGE);

        GLUtils.texImage2D(GLES30.GL_TEXTURE_2D, 0, bitmap, 0);
        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, 0);
        bitmap.recycle();

        return textureId;
    }

    public void handleTouchDrag(float deltaX, float deltaY) {
        xRotation += deltaX / 16f;
        yRotation += deltaY / 16f;

//        if (yRotation < -90) {
//            yRotation = -90;
//        } else if (yRotation > 90) {
//            yRotation = 90;
//        }
        if (yRotation < -360) {
            yRotation %= 360;
        } else if (yRotation > 360) {
            yRotation %= 360;
        }
    }
}
