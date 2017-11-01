package com.baofeng.mojing.MojingTextureBaker;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLES30;
import android.opengl.GLUtils;
import android.util.Log;

import com.google.vr.sdk.base.Eye;
import com.google.vr.sdk.base.GvrView;
import com.google.vr.sdk.base.HeadTransform;
import com.google.vr.sdk.base.Viewport;
import com.mapleaf.testbaker.R;

import java.io.InputStream;
import java.nio.IntBuffer;

import javax.microedition.khronos.egl.EGLConfig;

public class MojingTextureRender implements GvrView.StereoRenderer {
    int m_iBKColor;
    long m_hTextureBakerHandle = 0;
    Activity m_Context;
    public MojingTextureBaker m_Baker;

    // Cubemap
    int m_iTextureID_CUBE = 0;
    int m_iFboID_CUBE  = 0;

    int m_iBKTextureSize = 0;
    int m_iBKTextureSizeNew = 0;

    int[] m_iFOB = {0};
    int[] m_iBKTexture = {0};
    String m_sScript = "";
    private void LOGI(String s)
    {
        Log.i("MojingTextureRender" , s);
    }
    public MojingTextureRender(Activity activity)
    {
        m_Context = activity;
    }

    private void InitCubeMap()
    {
        int[] TID = new int[1];
        GLES30.glGenTextures(1 , TID , 0);
        m_iTextureID_CUBE = TID[0];
        GLES30.glBindTexture(GLES30.GL_TEXTURE_CUBE_MAP, m_iTextureID_CUBE);
        for (int i = 0 ; i < 6 ; i++)
        {
            GLES30.glTexImage2D(GLES30.GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GLES30.GL_RGBA,1024, 1024, 0, GLES30.GL_RGBA, GLES30.GL_UNSIGNED_BYTE, null );
        }
        GLES30.glTexParameteri(GLES30.GL_TEXTURE_CUBE_MAP, GLES30.GL_TEXTURE_WRAP_S,GLES30.GL_REPEAT);
        GLES30.glTexParameteri(GLES30.GL_TEXTURE_CUBE_MAP, GLES30.GL_TEXTURE_WRAP_T,GLES30.GL_REPEAT);
        GLES30.glTexParameteri(GLES30.GL_TEXTURE_CUBE_MAP, GLES30.GL_TEXTURE_WRAP_R,GLES30.GL_REPEAT);
        GLES30.glTexParameteri(GLES30.GL_TEXTURE_CUBE_MAP, GLES30.GL_TEXTURE_MAG_FILTER,GLES30.GL_NEAREST);
        GLES30.glTexParameteri(GLES30.GL_TEXTURE_CUBE_MAP, GLES30.GL_TEXTURE_MIN_FILTER,GLES30.GL_NEAREST);
        GLES30.glBindTexture(GLES30.GL_TEXTURE_CUBE_MAP, 0);

        if (m_iFboID_CUBE == 0)
        {
            IntBuffer buf =IntBuffer.allocate(1);
            GLES30.glGenFramebuffers(1, buf);
            m_iFboID_CUBE = buf.get(0);;
        }
    }

    private void DrawCubeMap()
    {
        GLES30.glBindFramebuffer(GLES30.GL_FRAMEBUFFER, m_iFOB[0]);
        for (int i = 0 ; i < 6 ; i++)
        {
            GLES30.glFramebufferTexture2D(GLES30.GL_FRAMEBUFFER, GLES30.GL_COLOR_ATTACHMENT0, GLES30.GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_iTextureID_CUBE, 0);
            GLES30.glClearColor(i%2 , (i % 3) / 3.0f ,i / 5.0f,1);
            GLES30.glClear( GLES30.GL_DEPTH_BUFFER_BIT | GLES30.GL_COLOR_BUFFER_BIT);
        }
        GLES30.glBindFramebuffer(GLES30.GL_FRAMEBUFFER , 0);
    }

    private void RebuildTexture(int iNewTextureSize)
    {
        LOGI("RebuildTexture - Begin");
        if (m_iBKTexture[0] != 0 && GLES30.glIsTexture(m_iBKTexture[0]))
        {
            //IntBuffer buf =IntBuffer.allocate(1);
            //buf.put(m_iBKTexture[0]);
            GLES30.glDeleteTextures(1 , m_iBKTexture , 0);
            //m_iBKTexture[0] = 0;
        }
        m_iBKTexture[0] = 0;
        GLES30.glGenTextures(1, m_iBKTexture, 0);
        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, m_iBKTexture[0]);

        GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MIN_FILTER,GLES30.GL_LINEAR);
        GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MAG_FILTER,GLES30.GL_LINEAR);
        GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_S,GLES30.GL_CLAMP_TO_EDGE);
        GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_T,GLES30.GL_CLAMP_TO_EDGE);
        GLES30.glTexImage2D(GLES30.GL_TEXTURE_2D, 0, GLES30.GL_RGBA, iNewTextureSize, iNewTextureSize, 0, GLES30.GL_RGBA,  GLES30.GL_UNSIGNED_BYTE, null);
        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, 0);

        m_iBKTextureSize = iNewTextureSize;
        LOGI("RebuildTexture - End");
    }
    private void ClearTexture()
    {
        LOGI("ClearTexture - Begin");
        if (m_iFOB[0] == 0)
        {
            IntBuffer buf =IntBuffer.allocate(1);
            GLES30.glGenFramebuffers(1, buf);
            m_iFOB[0] = buf.get(0);
        }

        GLES30.glBindFramebuffer(GLES30.GL_FRAMEBUFFER, m_iFOB[0]);
        GLES30.glFramebufferTexture2D(GLES30.GL_FRAMEBUFFER, GLES30.GL_COLOR_ATTACHMENT0, GLES30.GL_TEXTURE_2D, m_iBKTexture[0], 0);
        int status = GLES30.glCheckFramebufferStatus(GLES30.GL_FRAMEBUFFER);
        if (status == GLES30.GL_FRAMEBUFFER_COMPLETE)
        {
            int []iLastVierwPort = new int[4];
            GLES30.glGetIntegerv(GLES30.GL_VIEWPORT,iLastVierwPort  ,0);
            GLES30.glViewport(0, 0, m_iBKTextureSize, m_iBKTextureSize);
            GLES30.glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
            GLES30.glClear(GLES30.GL_COLOR_BUFFER_BIT);
            GLES30.glBindFramebuffer(GLES30.GL_FRAMEBUFFER, 0);
            if (iLastVierwPort[2] > 0 && iLastVierwPort[3] > 0)
                GLES30.glViewport(iLastVierwPort[0] , iLastVierwPort[1],iLastVierwPort[2],iLastVierwPort[3]);
        }

        LOGI("ClearTexture - End");
    }

    public void testDoBaker(){
        int[] MaxTextureSize = new int[1];
        GLES30.glGetIntegerv(GLES30.GL_MAX_TEXTURE_SIZE , MaxTextureSize , 0);
        m_hTextureBakerHandle = m_Baker.BeginBaker(MaxTextureSize[0]);

            Log.d("m_hTextureBakerHandle", "add: ");
            long tmBegin = System.currentTimeMillis();
            AppendTexture(m_hTextureBakerHandle , R.mipmap.ic_launcher);


            LOGI(" Append time(All) = " + (System.currentTimeMillis() -  tmBegin) + " ms");

            int iNewSize = m_Baker.GetBakerTextureSize((int)m_hTextureBakerHandle);
            if (iNewSize  > 0)
            {
                m_iBKTextureSizeNew = iNewSize ;
                LOGI(" BKTextureSizeNew = " + m_iBKTextureSizeNew);
            }

    }

    public int AppendTexture(long hHandle , int iResourceID)
    {

        InputStream is = m_Context.getResources().openRawResource(iResourceID);//mContext.getResources().openRawResource(R.drawable.strip);
        Bitmap bitmap;
        bitmap = BitmapFactory.decodeStream(is);
        int iWidth = bitmap.getWidth();
        int iHeight = bitmap.getHeight();
        int[] textures = new int[1];
        GLES30.glGenTextures(1, textures, 0);

        int textureId=textures[0];
        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, textureId);

        GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MIN_FILTER,GLES30.GL_LINEAR);
        GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MAG_FILTER,GLES30.GL_LINEAR);
        GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_S,GLES30.GL_CLAMP_TO_EDGE);
        GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_T,GLES30.GL_CLAMP_TO_EDGE);
        GLUtils.texImage2D( GLES30.GL_TEXTURE_2D, 0, bitmap, 0);
        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, 0);
        bitmap.recycle();

        long tmBegin = System.currentTimeMillis();


        int iRet = m_Baker.AddTexture((int)hHandle , textureId , 0 ,0, 0 , iWidth , iHeight);
        LOGI(" Append time = " + (System.currentTimeMillis() -  tmBegin) + " ms");
        return iRet;
    }



    @Override
    public void onNewFrame(HeadTransform headTransform) {

    }

    @Override
    public void onDrawEye(Eye eye) {
        int []iLastVierwPort = new int[4];

        GLES30.glGetIntegerv(GLES30.GL_VIEWPORT,iLastVierwPort  ,0);
        GLES30.glViewport(1280 * (eye.getType() - 1) , 0 , 1280 , 1440);
        GLES30.glClearColor(m_iBKColor / 256.0f ,0,0,1);
        GLES30.glClear(  GLES30.GL_COLOR_BUFFER_BIT);
        m_iBKColor = (m_iBKColor + 1)% 257;
        if (m_iBKColor == 0)
        {
            m_iBKTextureSize = 0;
        }
        if (m_iBKTextureSizeNew != m_iBKTextureSize)
        {
            RebuildTexture(m_iBKTextureSizeNew);
            ClearTexture();
            m_sScript = m_Baker.BakerTexture((int)m_hTextureBakerHandle ,m_iBKTexture[0] , true);
        }

        /*if (0 == m_iTextureID_CUBE)
        {
            InitCubeMap();
            DrawCubeMap();
        }*/
        GLES30.glViewport(iLastVierwPort[0],iLastVierwPort[1],iLastVierwPort[2],iLastVierwPort[3]);
    }

    @Override
    public void onFinishFrame(Viewport viewport) {

    }

    @Override
    public void onSurfaceChanged(int i, int i1) {

    }

    @Override
    public void onSurfaceCreated(EGLConfig eglConfig) {
        testDoBaker();
    }

    @Override
    public void onRendererShutdown() {

    }
}