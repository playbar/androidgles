package com.baofeng.mojing.MojingTextureBaker;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLES20;
import android.opengl.GLES30;
import android.opengl.GLSurfaceView;
import android.opengl.GLUtils;
import android.opengl.Matrix;

import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;
import java.util.Vector;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by zhangxingang on 2017/9/11.
 */

public class MojingGLSurfaceViewRender implements GLSurfaceView.Renderer {
    private Context mContext;
    private int mTextureId;
    private byte[] mData;
    private int mWidth;
    private int mHeight;
    private int mHandle;

    private static final String VERTEX_SHADER =
            "uniform mat4 uMVPMatrix;" +
                    "attribute vec4 vPosition;" +
                    "attribute vec2 a_texCoord;" +
                    "varying vec2 v_texCoord;" +
                    "void main() {" +
                    "  gl_Position = uMVPMatrix * vPosition;" +
                    "  v_texCoord = a_texCoord;" +
                    "}";
    private static final String FRAGMENT_SHADER =
            "precision mediump float;" +
                    "varying vec2 v_texCoord;" +
                    "uniform sampler2D s_texture;" +
                    "void main() {" +
                    "  gl_FragColor = texture2D(s_texture, v_texCoord);" +
                    "}";

    private static final float[] VERTEX = {   // in counterclockwise order:
            1, 1, 0,   // top right
            -1, 1, 0,  // top left
            -1, -1, 0, // bottom left
            1, -1, 0,  // bottom right
    };
    private static final float[] TEX_VERTEX = {   // in clockwise order:
            1, 0,  // bottom right
            0, 0,  // bottom left
            0, 1,  // top left
            1, 1,  // top right
    };
    //    private static final float[] TEX_VERTEX = {   // in clockwise order:
//            0.0f,0.0f,
//            0.0f,1.0f,
//            1.0f,0.0f,
//            1.0f,1.0f,
//    };
    private static final short[] VERTEX_INDEX = {
            0, 1, 2, 0, 2, 3
    };
    //    private static final float[] TEX_VERTEX = {   // in clockwise order:
//            0.5f, 0,  // bottom right
//            0, 0,  // bottom left
//            0, 0.5f,  // top left
//            0.5f, 0.5f,  // top right
//    };
    private final FloatBuffer mVertexBuffer;
    private final FloatBuffer mTexVertexBuffer;
    private final ShortBuffer mVertexIndexBuffer;
    private int mProgram;
    private int mPositionHandle;
    private int mMatrixHandle;
    private int mTexCoordHandle;
    private int mTexSamplerHandle;
    private int mTexName;
    private Vector<Integer> mTextureVector;

    private final float[] mMVPMatrix = new float[16];

    public MojingGLSurfaceViewRender(Context context) {
        mContext = context;
        mVertexBuffer = ByteBuffer.allocateDirect(VERTEX.length * 4)
                .order(ByteOrder.nativeOrder())
                .asFloatBuffer()
                .put(VERTEX);
        mVertexBuffer.position(0);
        mVertexIndexBuffer = ByteBuffer.allocateDirect(VERTEX_INDEX.length * 2)
                .order(ByteOrder.nativeOrder())
                .asShortBuffer()
                .put(VERTEX_INDEX);
        mVertexIndexBuffer.position(0);
        mTexVertexBuffer = ByteBuffer.allocateDirect(TEX_VERTEX.length * 4)
                .order(ByteOrder.nativeOrder())
                .asFloatBuffer()
                .put(TEX_VERTEX);
        mTexVertexBuffer.position(0);
    }

    static int loadShader(int type, String shaderCode) {
        int shader = GLES30.glCreateShader(type);
        GLES30.glShaderSource(shader, shaderCode);
        GLES30.glCompileShader(shader);
        return shader;
    }

    @Override
    public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
//        mTextureVector = new Vector<Integer>();
//        mTextureId = initImageTexture(R.drawable.ic_launcher, true);
//        mTextureVector.add(mTextureId);
//        mTextureId = initImageTexture(R.drawable.nav_logo, false);
//        mTextureVector.add(mTextureId);


        int iMaxTextureSize[] = new int[1];
        GLES30.glGetIntegerv(GLES30.GL_MAX_TEXTURE_SIZE, iMaxTextureSize, 0);
        int newid[] = new int[1];
        GLES30.glGenTextures(1, newid, 0);

        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, newid[0]);
        GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MIN_FILTER, GLES30.GL_LINEAR);//线性过滤差值
        GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MAG_FILTER, GLES30.GL_LINEAR);
        GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_S, GLES30.GL_CLAMP_TO_EDGE);//纹理边缘延伸效果
        GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_T, GLES30.GL_CLAMP_TO_EDGE);
        iMaxTextureSize[0] = iMaxTextureSize[0] > 8192 ? iMaxTextureSize[0] / 2 : 8192;
        GLES20.glTexImage2D(GLES20.GL_TEXTURE_2D, 0, GLES20.GL_RGBA, iMaxTextureSize[0], iMaxTextureSize[0], 0, GLES20.GL_RGBA, GLES20.GL_UNSIGNED_BYTE, null);
        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, 0);


        mHandle = MojingTextureBaker.BeginBaker(iMaxTextureSize[0]);
        initImage();

        String sRet = MojingTextureBaker.BakerTexture(mHandle, newid[0], false);

//        int framebuffer[] = new int[1];
//        GLES30.glGenFramebuffers(1, framebuffer, 0);
//        GLES30.glBindFramebuffer(GLES30.GL_READ_FRAMEBUFFER, framebuffer[0]);
//
//        GLES30.glBlendFunc(GLES30.GL_SRC_ALPHA, GLES30.GL_ONE_MINUS_SRC_ALPHA);
//        GLES30.glEnable(GLES30.GL_BLEND);
//        GLES30.glFramebufferTexture2D(GLES30.GL_READ_FRAMEBUFFER, GLES20.GL_COLOR_ATTACHMENT0,
//                GLES30.GL_TEXTURE_2D, newid[0], 0);
//        int iTempTexture[] = new int[1];
//        GLES30.glGenTextures(1, iTempTexture, 0);
//        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, iTempTexture[0]);
//        GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MIN_FILTER, GLES30.GL_LINEAR);//线性过滤差值
//        GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MAG_FILTER, GLES30.GL_LINEAR);
//        GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_S, GLES30.GL_CLAMP_TO_EDGE);//纹理边缘延伸效果
//        GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_T, GLES30.GL_CLAMP_TO_EDGE);
//        GLES20.glTexImage2D(GLES20.GL_TEXTURE_2D, 0, GLES20.GL_RGBA, 1024, 1024, 0, GLES20.GL_RGBA, GLES20.GL_UNSIGNED_BYTE, null);
//        GLES30.glCopyTexSubImage2D(GLES30.GL_TEXTURE_2D, 0, 0, 0, 0, 0, 1024, 1024);
//        GLES30.glBindFramebuffer(GLES30.GL_FRAMEBUFFER, 0);
//        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, 0);




       GLES30.glClearColor(0.7f, 0.7f, 0.7f, 0.7f);

        GLES30.glClear(GLES30.GL_COLOR_BUFFER_BIT);

        //mTextureId = GetTextureId();
        //GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, mTextureId);
//        int newid[] = new int[1];
//        GLES30.glGenTextures(1, newid, 0);
//
//        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, newid[0]);
//        GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MIN_FILTER, GLES30.GL_LINEAR);//线性过滤差值
//        GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MAG_FILTER, GLES30.GL_LINEAR);
//        GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_S, GLES30.GL_CLAMP_TO_EDGE);//纹理边缘延伸效果
//        GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_T, GLES30.GL_CLAMP_TO_EDGE);
//
//        GLES20.glTexImage2D(GLES20.GL_TEXTURE_2D, 0, GLES20.GL_RGBA, 144, 144, 0, GLES20.GL_RGBA, GLES20.GL_UNSIGNED_BYTE, null);
//        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, 0);

        //GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, 0);
//        int framebuffer[] = new int[1];
//        GLES30.glGenFramebuffers(1, framebuffer, 0);
//        GLES30.glBindFramebuffer(GLES30.GL_READ_FRAMEBUFFER, framebuffer[0]);
//
//        GLES30.glBlendFunc(GLES30.GL_SRC_ALPHA, GLES30.GL_ONE_MINUS_SRC_ALPHA);
//        GLES30.glEnable(GLES30.GL_BLEND);
//
//        GLES30.glFramebufferTexture2D(GLES30.GL_READ_FRAMEBUFFER, GLES20.GL_COLOR_ATTACHMENT0,
//                GLES30.GL_TEXTURE_2D, id, 0);
//        //GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, mTextureVector.get(0));
//        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, newid[0]);
//        //GLES30.glClearColor(0.5f, 0.5f, 0.5f, 0);
//        //GLES30.glClear(GLES30.GL_COLOR_BUFFER_BIT);
//
//        //GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, mTextureVector.get(1));
//        //GLES30.glCopyTexImage2D(GLES30.GL_TEXTURE_2D, 0, 0, 0, 0, 0, mWidth, mHeight);
//        GLES30.glCopyTexSubImage2D(GLES30.GL_TEXTURE_2D, 0, 0, 0, 0, 0, 144, 144);
//        GLES30.glBindFramebuffer(GLES30.GL_FRAMEBUFFER, 0);
//        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, 0);

        //GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, iTempTexture[0]);
        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, newid[0]);

        // GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, mTextureVector.get(1));

        mProgram = GLES30.glCreateProgram();
        int vertexShader = loadShader(GLES30.GL_VERTEX_SHADER, VERTEX_SHADER);
        int fragmentShader = loadShader(GLES30.GL_FRAGMENT_SHADER, FRAGMENT_SHADER);
        GLES30.glAttachShader(mProgram, vertexShader);
        GLES30.glAttachShader(mProgram, fragmentShader);
        GLES30.glLinkProgram(mProgram);

        GLES30.glUseProgram(mProgram);
        mPositionHandle = GLES30.glGetAttribLocation(mProgram, "vPosition");
        mTexCoordHandle = GLES30.glGetAttribLocation(mProgram, "a_texCoord");
        mMatrixHandle = GLES30.glGetUniformLocation(mProgram, "uMVPMatrix");
        mTexSamplerHandle = GLES30.glGetUniformLocation(mProgram, "s_texture");

        GLES30.glEnableVertexAttribArray(mPositionHandle);
        //传入顶点坐标
        GLES30.glVertexAttribPointer(mPositionHandle, 3, GLES30.GL_FLOAT, false,
                12, mVertexBuffer);

        //传入纹理坐标
        GLES30.glEnableVertexAttribArray(mTexCoordHandle);
        GLES30.glVertexAttribPointer(mTexCoordHandle, 2, GLES30.GL_FLOAT, false, 0,
                mTexVertexBuffer);
        //GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, 0);

    }



    @Override
    public void onSurfaceChanged(GL10 gl10, int width, int height) {
        GLES30.glViewport(0, 0, width, height);

        Matrix.perspectiveM(mMVPMatrix, 0, 45, (float) width / height, 0.1f, 100f);
        //Matrix.translateM(mMVPMatrix, 0, 0f, 0f, -5f);
    }

    @Override
    public void onDrawFrame(GL10 gl10) {


        GLES30.glClear(GLES30.GL_COLOR_BUFFER_BIT);

        GLES30.glUniformMatrix4fv(mMatrixHandle, 1, false, mMVPMatrix, 0);
        GLES30.glUniform1i(mTexSamplerHandle, 0);

        // 用 glDrawElements 来绘制，mVertexIndexBuffer 指定了顶点绘制顺序
        GLES30.glDrawElements(GLES30.GL_TRIANGLES, VERTEX_INDEX.length,
                GLES30.GL_UNSIGNED_SHORT, mVertexIndexBuffer);
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
        MojingTextureBaker.AddTexture(handle, textureId, 0, 0, bitmap.getWidth(), bitmap.getHeight());

        if (flag) {
            mData = new byte[4 * bitmap.getWidth() * bitmap.getHeight()];
            mWidth = bitmap.getWidth();
            mHeight = bitmap.getHeight();
        }

        bitmap.recycle();

        return textureId;
    }

    private native int GLTest(int iTextureId);

    private native int GetTextureId();

    private void initImage() {
        int id = initImageTexture(mHandle, R.drawable.ic_launcher, false);
        initImageTexture(mHandle , R.drawable.choose_icon_remember_highlight,false);
        initImageTexture(mHandle , R.drawable.choose_icon_simple,false);
        initImageTexture(mHandle , R.drawable.choose_icon_simple_highlight,false);
        initImageTexture(mHandle , R.drawable.clear_btn_normal,false);
        initImageTexture(mHandle , R.drawable.default_fly_screen_video,false);
        initImageTexture(mHandle , R.drawable.devlist_loading_icon,false);
        initImageTexture(mHandle , R.drawable.dialog_success_icon,false);
        initImageTexture(mHandle , R.drawable.empty_icon_warning,false);
        initImageTexture(mHandle , R.drawable.empty_img_subscribe_icon,false);
        initImageTexture(mHandle , R.drawable.enter_blueteeth_img3,false);
        initImageTexture(mHandle , R.drawable.enter_blueteeth_usb,false);
        initImageTexture(mHandle , R.drawable.enter_control_icon_hand,false);
        initImageTexture(mHandle , R.drawable.enter_control_icon_head,false);
        initImageTexture(mHandle , R.drawable.enter_icon_scan,false);
        initImageTexture(mHandle , R.drawable.enter_icon_setting,false);
        initImageTexture(mHandle , R.drawable.enter_img_phone,false);
        initImageTexture(mHandle , R.drawable.fast_forward_icon,false);
        initImageTexture(mHandle , R.drawable.fly_device_item,false);
        initImageTexture(mHandle , R.drawable.fly_device_item_pressed,false);
        initImageTexture(mHandle , R.drawable.fly_screen_device_search_nor,false);
        initImageTexture(mHandle , R.drawable.fly_screen_device_search_selected,false);
        initImageTexture(mHandle , R.drawable.fly_screen_guide_img_3,false);
        initImageTexture(mHandle , R.drawable.fly_screen_search_background,false);
        initImageTexture(mHandle , R.drawable.fly_screen_search_foreground,false);
        initImageTexture(mHandle , R.drawable.gameinf_icon_share_normal,false);
        initImageTexture(mHandle , R.drawable.game_rank_list_no_icon,false);
        initImageTexture(mHandle , R.drawable.guide_button,false);
        initImageTexture(mHandle , R.drawable.guide_icon1_highlight,false);
        initImageTexture(mHandle , R.drawable.guide_icon1_normal,false);
        initImageTexture(mHandle , R.drawable.guide_icon2_highlight,false);
        initImageTexture(mHandle , R.drawable.guide_icon2_normal,false);
        initImageTexture(mHandle , R.drawable.guide_icon_choose_highlight,false);
        initImageTexture(mHandle , R.drawable.guide_icon_choose_normal,false);
        initImageTexture(mHandle , R.drawable.handle_back,false);
        initImageTexture(mHandle , R.drawable.handle_menu,false);
        initImageTexture(mHandle , R.drawable.help_feedback_pressed,false);
        initImageTexture(mHandle , R.drawable.help_icon_successful,false);
        initImageTexture(mHandle , R.drawable.icon_cate_label,false);
        initImageTexture(mHandle , R.drawable.icon_choose_normal,false);
        initImageTexture(mHandle , R.drawable.icon_choose_selected,false);
        initImageTexture(mHandle , R.drawable.icon_close_black,false);
        initImageTexture(mHandle , R.drawable.icon_motion_handle_has_match,false);
        initImageTexture(mHandle , R.drawable.icon_motion_handle_no_match,false);
        initImageTexture(mHandle , R.drawable.icon_search,false);
        initImageTexture(mHandle , R.drawable.icon_selected,false);
        initImageTexture(mHandle , R.drawable.icon_unlock,false);
        initImageTexture(mHandle , R.drawable.icon_video_360,false);
        initImageTexture(mHandle , R.drawable.ic_launcher,false);
        initImageTexture(mHandle , R.drawable.img_common_problem,false);
        initImageTexture(mHandle , R.drawable.img_default_1n_cross,false);
        initImageTexture(mHandle , R.drawable.img_default_2n_cross,false);
        initImageTexture(mHandle , R.drawable.img_default_2n_vertical,false);
        initImageTexture(mHandle , R.drawable.img_default_3n_cross,false);
        initImageTexture(mHandle , R.drawable.img_default_3n_vertical,false);
        initImageTexture(mHandle , R.drawable.img_default_4n,false);
        initImageTexture(mHandle , R.drawable.img_default_banner,false);
        initImageTexture(mHandle , R.drawable.img_default_icon_classification,false);
        initImageTexture(mHandle , R.drawable.invr_icon_back_normal,false);
        initImageTexture(mHandle , R.drawable.invr_image_glasses1,false);
        initImageTexture(mHandle , R.drawable.invr_image_glasses2,false);
        initImageTexture(mHandle , R.drawable.invr_image_phone,false);
        initImageTexture(mHandle , R.drawable.iv_file_image_360_label,false);
        initImageTexture(mHandle , R.drawable.iv_file_image_360_label_bg,false);
        initImageTexture(mHandle , R.drawable.label_activity,false);
        initImageTexture(mHandle , R.drawable.label_first,false);
        initImageTexture(mHandle , R.drawable.label_live,false);
        initImageTexture(mHandle , R.drawable.label_sole,false);
        initImageTexture(mHandle , R.drawable.label_toukong,false);
        initImageTexture(mHandle , R.drawable.laber_dan,false);
        initImageTexture(mHandle , R.drawable.laber_guanfang,false);
        initImageTexture(mHandle , R.drawable.laber_leixing,false);
        initImageTexture(mHandle , R.drawable.laber_sanfang,false);
        initImageTexture(mHandle , R.drawable.laber_shuang,false);
        initImageTexture(mHandle , R.drawable.laber_wuguanggao,false);
        initImageTexture(mHandle , R.drawable.laber_youguanggao,false);
        initImageTexture(mHandle , R.drawable.laber_zhuanti,false);
        initImageTexture(mHandle , R.drawable.live_icon,false);
        initImageTexture(mHandle , R.drawable.loading_10_small,false);
        initImageTexture(mHandle , R.drawable.local_airplay_icon_screen,false);
        initImageTexture(mHandle , R.drawable.local_airplay_icon_share,false);
        initImageTexture(mHandle , R.drawable.local_airplay_share_icon_help,false);
        initImageTexture(mHandle , R.drawable.local_airplay_share_icon_loading,false);
        initImageTexture(mHandle , R.drawable.local_airplay_share_icon_reload,false);
        initImageTexture(mHandle , R.drawable.local_airplay_share_icon_win,false);
        initImageTexture(mHandle , R.drawable.local_download_null_icon,false);
        initImageTexture(mHandle , R.drawable.local_file_browse_back_icon,false);
        initImageTexture(mHandle , R.drawable.local_file_icon,false);
        initImageTexture(mHandle , R.drawable.local_file_icon_back,false);
        initImageTexture(mHandle , R.drawable.local_file_image_arrow,false);
        initImageTexture(mHandle , R.drawable.local_file_image_http,false);
        initImageTexture(mHandle , R.drawable.local_file_list_icon_fly,false);
        initImageTexture(mHandle , R.drawable.local_file_list_icon_http,false);
        initImageTexture(mHandle , R.drawable.local_file_list_icon_network,false);
        initImageTexture(mHandle , R.drawable.local_file_switchoff,false);
        initImageTexture(mHandle , R.drawable.local_file_switchon,false);
        initImageTexture(mHandle , R.drawable.local_folder_icon,false);
        initImageTexture(mHandle , R.drawable.local_icon_sort_normal,false);
        initImageTexture(mHandle , R.drawable.local_image_default,false);
        initImageTexture(mHandle , R.drawable.local_screen_img_0,false);
        initImageTexture(mHandle , R.drawable.local_screen_img_help_1,false);
        initImageTexture(mHandle , R.drawable.local_screen_img_help_2,false);
        initImageTexture(mHandle , R.drawable.local_screen_img_help_3,false);
        initImageTexture(mHandle , R.drawable.local_screen_img_nothing,false);
        initImageTexture(mHandle , R.drawable.local_text_icon,false);
        initImageTexture(mHandle , R.drawable.local_video_icon,false);
        initImageTexture(mHandle , R.drawable.local_video_menu,false);
        initImageTexture(mHandle , R.drawable.local_video_perview_icon_close,false);
        initImageTexture(mHandle , R.drawable.local_video_perview_icon_play,false);
        initImageTexture(mHandle , R.drawable.local_video_perview_icon_vrplay,false);
        initImageTexture(mHandle , R.drawable.local_video__folder,false);
        initImageTexture(mHandle , R.drawable.market_icon_game,false);
        initImageTexture(mHandle , R.drawable.market_ranking_icon_1,false);
        initImageTexture(mHandle , R.drawable.mask_bg,false);
        initImageTexture(mHandle , R.drawable.microtutor_pen_progressdot,false);
        initImageTexture(mHandle , R.drawable.mj_5_icon,false);
        initImageTexture(mHandle , R.drawable.mj_contraller_pop_icon,false);
        initImageTexture(mHandle , R.drawable.mj_pause_checked,false);
        initImageTexture(mHandle , R.drawable.mj_pause_unchecked,false);
        initImageTexture(mHandle , R.drawable.mj_play_checked,false);
        initImageTexture(mHandle , R.drawable.mj_play_unchecked,false);
        initImageTexture(mHandle , R.drawable.mj_rank_icon_top1,false);
        initImageTexture(mHandle , R.drawable.mj_rank_icon_top2,false);
        initImageTexture(mHandle , R.drawable.mj_rank_icon_top3,false);
        initImageTexture(mHandle , R.drawable.mj_rank_title_bg,false);
        initImageTexture(mHandle , R.drawable.mojing_default,false);
        initImageTexture(mHandle , R.drawable.motion_handle_home,false);
        initImageTexture(mHandle , R.drawable.motion_handle_touch,false);
        initImageTexture(mHandle , R.drawable.my_about_img_logo,false);
        initImageTexture(mHandle , R.drawable.my_icon_bean,false);
        initImageTexture(mHandle , R.drawable.my_icon_cardboard,false);
        initImageTexture(mHandle , R.drawable.my_icon_coin,false);
        initImageTexture(mHandle , R.drawable.my_icon_equipment,false);
        initImageTexture(mHandle , R.drawable.my_icon_handle,false);
        initImageTexture(mHandle , R.drawable.my_icon_help,false);
        initImageTexture(mHandle , R.drawable.my_icon_mission,false);
        initImageTexture(mHandle , R.drawable.my_icon_otherhandle,false);
        initImageTexture(mHandle , R.drawable.my_icon_pay,false);
        initImageTexture(mHandle , R.drawable.my_icon_question,false);
        initImageTexture(mHandle , R.drawable.my_icon_read,false);
        initImageTexture(mHandle , R.drawable.my_icon_scan,false);
        initImageTexture(mHandle , R.drawable.my_icon_set,false);
        initImageTexture(mHandle , R.drawable.my_icon_shop,false);
        initImageTexture(mHandle , R.drawable.my_icon_time,false);
        initImageTexture(mHandle , R.drawable.my_right_arrow,false);
        initImageTexture(mHandle , R.drawable.my_scan_close,false);
        initImageTexture(mHandle , R.drawable.my_scan_lightline,false);
        initImageTexture(mHandle , R.drawable.my_setting_switchoff,false);
        initImageTexture(mHandle , R.drawable.my_setting_switchon,false);
        initImageTexture(mHandle , R.drawable.nav_icon_add,false);
        initImageTexture(mHandle , R.drawable.nav_icon_arrow_down,false);
        initImageTexture(mHandle , R.drawable.nav_icon_arrow_up,false);
        initImageTexture(mHandle , R.drawable.nav_icon_back,false);
        initImageTexture(mHandle , R.drawable.nav_icon_back2,false);
        initImageTexture(mHandle , R.drawable.nav_icon_invr,false);
        initImageTexture(mHandle , R.drawable.nav_icon_invr_home,false);
        initImageTexture(mHandle , R.drawable.nav_icon_invr_white,false);
        initImageTexture(mHandle , R.drawable.nav_icon_me,false);
        initImageTexture(mHandle , R.drawable.nav_icon_search,false);
        initImageTexture(mHandle , R.drawable.nav_logo,false);
        initImageTexture(mHandle , R.drawable.playbar_progressbar,false);
        initImageTexture(mHandle , R.drawable.player_preview_button_play,false);
        initImageTexture(mHandle , R.drawable.player_preview_button_play_press,false);
        initImageTexture(mHandle , R.drawable.play_bg_control_hover_130_60,false);
        initImageTexture(mHandle , R.drawable.play_bg_control_hover_140_60,false);
        initImageTexture(mHandle , R.drawable.play_bg_control_hover_150_45,false);
        initImageTexture(mHandle , R.drawable.play_bg_control_hover_150_60,false);
        initImageTexture(mHandle , R.drawable.play_bg_control_hover_400_60,false);
        initImageTexture(mHandle , R.drawable.play_bg_control_hover_440_60,false);
        initImageTexture(mHandle , R.drawable.play_bg_control_hover_890_80,false);
        initImageTexture(mHandle , R.drawable.play_bg_control_normal_130_60,false);
        initImageTexture(mHandle , R.drawable.play_bg_control_normal_140_60,false);
        initImageTexture(mHandle , R.drawable.play_bg_control_normal_150_45,false);
        initImageTexture(mHandle , R.drawable.play_bg_control_normal_150_60,false);
        initImageTexture(mHandle , R.drawable.play_bg_control_normal_400_60,false);
        initImageTexture(mHandle , R.drawable.play_bg_control_normal_440_60,false);
        initImageTexture(mHandle , R.drawable.play_bg_control_normal_740_60,false);
        initImageTexture(mHandle , R.drawable.play_bg_control_normal_890_80,false);
        initImageTexture(mHandle , R.drawable.play_bg_scene_hover,false);
        initImageTexture(mHandle , R.drawable.play_bg_scene_normal,false);
        initImageTexture(mHandle , R.drawable.play_bg_scene_ph_cinema,false);
        initImageTexture(mHandle , R.drawable.play_bg_scene_ph_home,false);
        initImageTexture(mHandle , R.drawable.play_button_bg_ok_hover,false);
        initImageTexture(mHandle , R.drawable.play_button_bg_ok_normal,false);
        initImageTexture(mHandle , R.drawable.play_button_down_disable,false);
        initImageTexture(mHandle , R.drawable.play_button_down_hover,false);
        initImageTexture(mHandle , R.drawable.play_button_down_normal,false);
        initImageTexture(mHandle , R.drawable.play_button_up_disable,false);
        initImageTexture(mHandle , R.drawable.play_button_up_hover,false);
        initImageTexture(mHandle , R.drawable.play_button_up_normal,false);
        initImageTexture(mHandle , R.drawable.play_control_cursor_hover,false);
        initImageTexture(mHandle , R.drawable.play_control_cursor_normal,false);
        initImageTexture(mHandle , R.drawable.play_control_slider_bg,false);
        initImageTexture(mHandle , R.drawable.play_control_slider_progress,false);
        initImageTexture(mHandle , R.drawable.play_control_slider_progress_1,false);
        initImageTexture(mHandle , R.drawable.play_cursor,false);
        initImageTexture(mHandle , R.drawable.play_cursor_hover,false);
        initImageTexture(mHandle , R.drawable.play_icon_function_definition_click,false);
        initImageTexture(mHandle , R.drawable.play_icon_function_definition_disable,false);
        initImageTexture(mHandle , R.drawable.play_icon_function_definition_hover,false);
        initImageTexture(mHandle , R.drawable.play_icon_function_definition_normal,false);
        initImageTexture(mHandle , R.drawable.play_icon_function_model_click,false);
        initImageTexture(mHandle , R.drawable.play_icon_function_model_hover,false);
        initImageTexture(mHandle , R.drawable.play_icon_function_model_normal,false);
        initImageTexture(mHandle , R.drawable.play_icon_function_number_click,false);
        initImageTexture(mHandle , R.drawable.play_icon_function_number_disable,false);
        initImageTexture(mHandle , R.drawable.play_icon_function_number_hover,false);
        initImageTexture(mHandle , R.drawable.play_icon_function_number_normal,false);
        initImageTexture(mHandle , R.drawable.play_icon_function_pause_click,false);
        initImageTexture(mHandle , R.drawable.play_icon_function_pause_hover,false);
        initImageTexture(mHandle , R.drawable.play_icon_function_pause_normal,false);
        initImageTexture(mHandle , R.drawable.play_icon_function_play_click,false);
        initImageTexture(mHandle , R.drawable.play_icon_function_play_hover,false);
        initImageTexture(mHandle , R.drawable.play_icon_function_play_normal,false);
        initImageTexture(mHandle , R.drawable.play_icon_function_setting_click,false);
        initImageTexture(mHandle , R.drawable.play_icon_function_setting_hover,false);
        initImageTexture(mHandle , R.drawable.play_icon_function_setting_normal,false);
        initImageTexture(mHandle , R.drawable.play_icon_function_video_click,false);
        initImageTexture(mHandle , R.drawable.play_icon_function_video_disable,false);
        initImageTexture(mHandle , R.drawable.play_icon_function_video_hover,false);
        initImageTexture(mHandle , R.drawable.play_icon_function_video_normal,false);
        initImageTexture(mHandle , R.drawable.play_icon_function_voice_click,false);
        initImageTexture(mHandle , R.drawable.play_icon_function_voice_hover,false);
        initImageTexture(mHandle , R.drawable.play_icon_function_voice_normal,false);
        initImageTexture(mHandle , R.drawable.play_icon_loading,false);
        initImageTexture(mHandle , R.drawable.play_icon_model_180_click,false);
        initImageTexture(mHandle , R.drawable.play_icon_model_180_hover,false);
        initImageTexture(mHandle , R.drawable.play_icon_model_180_normal,false);
        initImageTexture(mHandle , R.drawable.play_icon_model_2d_click,false);
        initImageTexture(mHandle , R.drawable.play_icon_model_2d_hover,false);
        initImageTexture(mHandle , R.drawable.play_icon_model_2d_normal,false);
        initImageTexture(mHandle , R.drawable.play_icon_model_360_click,false);
        initImageTexture(mHandle , R.drawable.play_icon_model_360_hover,false);
        initImageTexture(mHandle , R.drawable.play_icon_model_360_normal,false);
        initImageTexture(mHandle , R.drawable.play_icon_model_3dside_click,false);
        initImageTexture(mHandle , R.drawable.play_icon_model_3dside_hover,false);
        initImageTexture(mHandle , R.drawable.play_icon_model_3dside_normal,false);
        initImageTexture(mHandle , R.drawable.play_icon_model_3dtop_click,false);
        initImageTexture(mHandle , R.drawable.play_icon_model_3dtop_hover,false);
        initImageTexture(mHandle , R.drawable.play_icon_model_3dtop_normal,false);
        initImageTexture(mHandle , R.drawable.play_icon_model_plane_click,false);
        initImageTexture(mHandle , R.drawable.play_icon_model_plane_hover,false);
        initImageTexture(mHandle , R.drawable.play_icon_model_plane_normal,false);
        initImageTexture(mHandle , R.drawable.play_icon_mute_hover1,false);
        initImageTexture(mHandle , R.drawable.play_icon_mute_normal,false);
        initImageTexture(mHandle , R.drawable.play_icon_setting_go_hover,false);
        initImageTexture(mHandle , R.drawable.play_icon_setting_go_normal,false);
        initImageTexture(mHandle , R.drawable.play_icon_setting_high_hover,false);
        initImageTexture(mHandle , R.drawable.play_icon_setting_high_normal,false);
        initImageTexture(mHandle , R.drawable.play_icon_setting_return_hover,false);
        initImageTexture(mHandle , R.drawable.play_icon_setting_return_normal,false);
        initImageTexture(mHandle , R.drawable.play_icon_setting_scene_hover,false);
        initImageTexture(mHandle , R.drawable.play_icon_setting_scene_normal,false);
        initImageTexture(mHandle , R.drawable.play_icon_setting_switch_disable,false);
        initImageTexture(mHandle , R.drawable.play_icon_setting_switch_hover,false);
        initImageTexture(mHandle , R.drawable.play_lock_button_lock_hover,false);
        initImageTexture(mHandle , R.drawable.play_lock_button_lock_normal,false);
        initImageTexture(mHandle , R.drawable.play_lock_button_unlock_hover,false);
        initImageTexture(mHandle , R.drawable.play_lock_button_unlock_normal,false);
        initImageTexture(mHandle , R.drawable.play_lock_tips_bg,false);
        initImageTexture(mHandle , R.drawable.play_menu_button_launch_hover,false);
        initImageTexture(mHandle , R.drawable.play_menu_button_launch_normal,false);
        initImageTexture(mHandle , R.drawable.play_menu_button_quit_hover,false);
        initImageTexture(mHandle , R.drawable.play_menu_button_quit_normal,false);
        initImageTexture(mHandle , R.drawable.play_menu_button_view_hover,false);
        initImageTexture(mHandle , R.drawable.play_menu_button_view_normal,false);
        initImageTexture(mHandle , R.drawable.play_rec_bg2_hover,false);
        initImageTexture(mHandle , R.drawable.play_rec_bg2_normal,false);
        initImageTexture(mHandle , R.drawable.play_rec_button_play_hover,false);
        initImageTexture(mHandle , R.drawable.play_rec_button_play_normal,false);
        initImageTexture(mHandle , R.drawable.play_rec_last_disable,false);
        initImageTexture(mHandle , R.drawable.play_rec_last_hover,false);
        initImageTexture(mHandle , R.drawable.play_rec_last_normal,false);
        initImageTexture(mHandle , R.drawable.play_rec_next_disable,false);
        initImageTexture(mHandle , R.drawable.play_rec_next_hover,false);
        initImageTexture(mHandle , R.drawable.play_rec_next_normal,false);
        initImageTexture(mHandle , R.drawable.play_rec_txt_mask_bg,false);
        initImageTexture(mHandle , R.drawable.play_slider_bg,false);
        initImageTexture(mHandle , R.drawable.play_slider_progress,false);
        initImageTexture(mHandle , R.drawable.play_toast_icon_fastforward,false);
        initImageTexture(mHandle , R.drawable.play_toast_icon_model,false);
        initImageTexture(mHandle , R.drawable.play_toast_icon_mute,false);
        initImageTexture(mHandle , R.drawable.play_toast_icon_rewind,false);
        initImageTexture(mHandle , R.drawable.play_toast_icon_voice,false);
        initImageTexture(mHandle , R.drawable.play_touch_bg_black,false);
        initImageTexture(mHandle , R.drawable.play_touch_cursor_normal,false);
        initImageTexture(mHandle , R.drawable.play_touch_icon_back_normal,false);
        initImageTexture(mHandle , R.drawable.play_touch_icon_double_normal,false);
        initImageTexture(mHandle , R.drawable.play_touch_icon_loading,false);
        initImageTexture(mHandle , R.drawable.play_touch_icon_lock_normal,false);
        initImageTexture(mHandle , R.drawable.play_touch_icon_pause_normal,false);
        initImageTexture(mHandle , R.drawable.play_touch_icon_play_normal,false);
        initImageTexture(mHandle , R.drawable.play_touch_icon_setting_normal,false);
        initImageTexture(mHandle , R.drawable.play_touch_icon_setting_vr,false);
        initImageTexture(mHandle , R.drawable.play_touch_icon_single_normal,false);
        initImageTexture(mHandle , R.drawable.play_touch_icon_unlock_normal,false);
        initImageTexture(mHandle , R.drawable.play_touch_mask_down,false);
        initImageTexture(mHandle , R.drawable.play_touch_mask_up,false);
        initImageTexture(mHandle , R.drawable.play_touch_model_icon_180_click,false);
        initImageTexture(mHandle , R.drawable.play_touch_model_icon_180_normal,false);
        initImageTexture(mHandle , R.drawable.play_touch_model_icon_2d_click,false);
        initImageTexture(mHandle , R.drawable.play_touch_model_icon_2d_normal,false);
        initImageTexture(mHandle , R.drawable.play_touch_model_icon_360_click,false);
        initImageTexture(mHandle , R.drawable.play_touch_model_icon_360_normal,false);
        initImageTexture(mHandle , R.drawable.play_touch_model_icon_3dside_click,false);
        initImageTexture(mHandle , R.drawable.play_touch_model_icon_3dside_normal,false);
        initImageTexture(mHandle , R.drawable.play_touch_model_icon_3dtop_click,false);
        initImageTexture(mHandle , R.drawable.play_touch_model_icon_3dtop_normal,false);
        initImageTexture(mHandle , R.drawable.play_touch_model_icon_plane_click,false);
        initImageTexture(mHandle , R.drawable.play_touch_model_icon_plane_normal,false);
        initImageTexture(mHandle , R.drawable.play_touch_number_icon_open_normal,false);
        initImageTexture(mHandle , R.drawable.play_touch_popup_icon_close_normal,false);
        initImageTexture(mHandle , R.drawable.play_touch_recommend_icon_loading,false);
        initImageTexture(mHandle , R.drawable.play_touch_setting_icon_change_disable,false);
        initImageTexture(mHandle , R.drawable.play_touch_setting_icon_change_normal,false);
        initImageTexture(mHandle , R.drawable.play_touch_toast_icon_light,false);
        initImageTexture(mHandle , R.drawable.play_touch_toast_icon_mute,false);
        initImageTexture(mHandle , R.drawable.play_touch_toast_icon_voice,false);
        initImageTexture(mHandle , R.drawable.play_video_button_play_hover,false);
        initImageTexture(mHandle , R.drawable.play_video_button_play_normal,false);
        initImageTexture(mHandle , R.drawable.play_video_local_bg_click,false);
        initImageTexture(mHandle , R.drawable.play_video_local_bg_empty,false);
        initImageTexture(mHandle , R.drawable.play_video_local_bg_hover,false);
        initImageTexture(mHandle , R.drawable.play_video_local_bg_mask,false);
        initImageTexture(mHandle , R.drawable.play_video_local_icon_play,false);
        initImageTexture(mHandle , R.drawable.play_video_local_ph_bg,false);
        initImageTexture(mHandle , R.drawable.play_video_online_bg_empty,false);
        initImageTexture(mHandle , R.drawable.play_video_online_bg_hover,false);
        initImageTexture(mHandle , R.drawable.play_video_online_bg_mask,false);
        initImageTexture(mHandle , R.drawable.play_video_online_continue_bg,false);
        initImageTexture(mHandle , R.drawable.play_video_online_continue_bg_mask,false);
        initImageTexture(mHandle , R.drawable.play_video_online_continue_ph_bg,false);
        initImageTexture(mHandle , R.drawable.play_video_online_ph_bg,false);
        initImageTexture(mHandle , R.drawable.play_volume_cursor_hover,false);
        initImageTexture(mHandle , R.drawable.play_volume_cursor_normal,false);
        initImageTexture(mHandle , R.drawable.play_volume_slider_bg,false);
        initImageTexture(mHandle , R.drawable.play_volume_slider_progress,false);
        initImageTexture(mHandle , R.drawable.play_welcome_image_handle,false);
        initImageTexture(mHandle , R.drawable.play_welcome_image_otherhandle,false);
        initImageTexture(mHandle , R.drawable.pubilc_tab_line_higlight,false);
        initImageTexture(mHandle , R.drawable.public_arrow_down,false);
        initImageTexture(mHandle , R.drawable.public_arrow_right,false);
        initImageTexture(mHandle , R.drawable.public_arrow_up,false);
        initImageTexture(mHandle , R.drawable.public_bg_gradient,false);
        initImageTexture(mHandle , R.drawable.public_bg_nav,false);
        initImageTexture(mHandle , R.drawable.public_label_jumu,false);
        initImageTexture(mHandle , R.drawable.public_label_ruanjian,false);
        initImageTexture(mHandle , R.drawable.public_label_youxi,false);
        initImageTexture(mHandle , R.drawable.public_label_zhibo,false);
        initImageTexture(mHandle , R.drawable.public_label_zhuanti,false);
        initImageTexture(mHandle , R.drawable.public_page_lighlight,false);
        initImageTexture(mHandle , R.drawable.public_page_normal,false);
        initImageTexture(mHandle , R.drawable.purple_round_bg,false);
        initImageTexture(mHandle , R.drawable.push_icon_close_normal,false);
        initImageTexture(mHandle , R.drawable.push_icon_cup,false);
        initImageTexture(mHandle , R.drawable.push_icon_moon,false);
        initImageTexture(mHandle , R.drawable.push_icon_pushlist,false);
        initImageTexture(mHandle , R.drawable.push_icon_up,false);
        initImageTexture(mHandle , R.drawable.push_list_icon_360,false);
        initImageTexture(mHandle , R.drawable.push_list_icon_play,false);
        initImageTexture(mHandle , R.drawable.push_logo,false);
        initImageTexture(mHandle , R.drawable.push_tips_bg,false);
        initImageTexture(mHandle , R.drawable.question_down,false);
        initImageTexture(mHandle , R.drawable.question_normal,false);
        initImageTexture(mHandle , R.drawable.recommend_content_default_bg,false);
        initImageTexture(mHandle , R.drawable.recommend_download,false);
        initImageTexture(mHandle , R.drawable.recommend_icon_play,false);
        initImageTexture(mHandle , R.drawable.recommend_icon_search,false);
        initImageTexture(mHandle , R.drawable.recommend_icon_suspended,false);
        initImageTexture(mHandle , R.drawable.recommend_nav_icon_vr,false);
        initImageTexture(mHandle , R.drawable.recommend_star,false);
        initImageTexture(mHandle , R.drawable.rec_handleplay_icon_close_normal,false);
        initImageTexture(mHandle , R.drawable.rec_icon_download_normal,false);
        initImageTexture(mHandle , R.drawable.rec_icon_download_ok_normal,false);
        initImageTexture(mHandle , R.drawable.rec_icon_play,false);
        initImageTexture(mHandle , R.drawable.rec_play_icon_close_normal,false);
        initImageTexture(mHandle , R.drawable.rec_play_mask_down,false);
        initImageTexture(mHandle , R.drawable.rec_play_mask_up,false);
        initImageTexture(mHandle , R.drawable.remote_controller_pause,false);
        initImageTexture(mHandle , R.drawable.remote_controller_volume,false);
        initImageTexture(mHandle , R.drawable.resource_dialog_failure_icon,false);
        initImageTexture(mHandle , R.drawable.res_fly_screen_fragment_file_img,false);
        initImageTexture(mHandle , R.drawable.res_local_btn_check_off_focused_holo_light,false);
        initImageTexture(mHandle , R.drawable.rewind_icon,false);
        initImageTexture(mHandle , R.drawable.save_waiting,false);
        initImageTexture(mHandle , R.drawable.save_warning,false);
        initImageTexture(mHandle , R.drawable.search_icon_delete,false);
        initImageTexture(mHandle , R.drawable.seekbar_play_icon,false);
        initImageTexture(mHandle , R.drawable.seekbar_progress_thumb,false);
        initImageTexture(mHandle , R.drawable.seekbar_suspended_icon,false);
        initImageTexture(mHandle , R.drawable.share_icon_discover,false);
        initImageTexture(mHandle , R.drawable.share_icon_kongjian,false);
        initImageTexture(mHandle , R.drawable.share_icon_link,false);
        initImageTexture(mHandle , R.drawable.share_icon_qq,false);
        initImageTexture(mHandle , R.drawable.share_icon_sina,false);
        initImageTexture(mHandle , R.drawable.share_icon_wechat,false);
        initImageTexture(mHandle , R.drawable.share_logo,false);
        initImageTexture(mHandle , R.drawable.sliding,false);
        initImageTexture(mHandle , R.drawable.smb_login_delete_checked,false);
        initImageTexture(mHandle , R.drawable.smb_login_delete_unchecked,false);
        initImageTexture(mHandle , R.drawable.surfaceview_tip_bg,false);
        initImageTexture(mHandle , R.drawable.tab_icon_more,false);
        initImageTexture(mHandle , R.drawable.tab_more_bg,false);
        initImageTexture(mHandle , R.drawable.toolbar_icon_amplification,false);
        initImageTexture(mHandle , R.drawable.toolbar_icon_home_highlight,false);
        initImageTexture(mHandle , R.drawable.toolbar_icon_home_normal,false);
        initImageTexture(mHandle , R.drawable.toolbar_icon_local_highlight,false);
        initImageTexture(mHandle , R.drawable.toolbar_icon_local_normal,false);
        initImageTexture(mHandle , R.drawable.toolbar_icon_market_highlight,false);
        initImageTexture(mHandle , R.drawable.toolbar_icon_market_normal,false);
        initImageTexture(mHandle , R.drawable.toolbar_icon_my_highlight,false);
        initImageTexture(mHandle , R.drawable.toolbar_icon_my_normal,false);
        initImageTexture(mHandle , R.drawable.toolbar_icon_play,false);
        initImageTexture(mHandle , R.drawable.toolbar_icon_rec_highlight,false);
        initImageTexture(mHandle , R.drawable.toolbar_icon_rec_normal,false);
        initImageTexture(mHandle , R.drawable.toolbar_icon_suspended,false);
        initImageTexture(mHandle , R.drawable.toolbar_icon_top_highlight,false);
        initImageTexture(mHandle , R.drawable.toolbar_icon_video_highlight,false);
        initImageTexture(mHandle , R.drawable.toolbar_icon_video_normal,false);
        initImageTexture(mHandle , R.drawable.umeng_convastion_reply,false);
        initImageTexture(mHandle , R.drawable.umeng_convationg_dialog_default_btn,false);
        initImageTexture(mHandle , R.drawable.umeng_convationg_dialog_pressed_btn,false);
        initImageTexture(mHandle , R.drawable.umeng_fb_logo,false);
        initImageTexture(mHandle , R.drawable.unity_back_icon,false);
        initImageTexture(mHandle , R.drawable.user_default_head_portrait,false);
        initImageTexture(mHandle , R.drawable.videoinf_button_download_icon_normal,false);
        initImageTexture(mHandle , R.drawable.videoinf_button_vrplay_bg_normal,false);
        initImageTexture(mHandle , R.drawable.videoinf_icon_close_normal,false);
        initImageTexture(mHandle , R.drawable.videoinf_icon_download_normal,false);
        initImageTexture(mHandle , R.drawable.videoinf_icon_download_ok_normal,false);
        initImageTexture(mHandle , R.drawable.videoinf_icon_more_normal,false);
        initImageTexture(mHandle , R.drawable.videoinf_icon_number,false);
        initImageTexture(mHandle , R.drawable.videoinf_icon_score,false);
        initImageTexture(mHandle , R.drawable.videoinf_icon_share_normal,false);
        initImageTexture(mHandle , R.drawable.videoinf_icon_vrplay_normal,false);
        initImageTexture(mHandle , R.drawable.videoinf_mask_bg,false);
        initImageTexture(mHandle , R.drawable.videoinf_rec_ph,false);
        initImageTexture(mHandle , R.drawable.video_3d,false);
        initImageTexture(mHandle , R.drawable.video_hot,false);
        initImageTexture(mHandle , R.drawable.video_icon_close,false);
        initImageTexture(mHandle , R.drawable.video_icon_play,false);
        initImageTexture(mHandle , R.drawable.video_new,false);
        initImageTexture(mHandle , R.drawable.video_vr,false);
        initImageTexture(mHandle , R.drawable.video_vr_3d,false);
        initImageTexture(mHandle , R.drawable.warning_icon,false);
        initImageTexture(mHandle , R.drawable.wifi_disconnect,false);
        initImageTexture(mHandle , R.drawable.zhifubao_icon,false);
    }
}
