package com.android.player;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.SurfaceTexture;
import android.media.MediaPlayer;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.Matrix;
import android.util.Log;
import android.view.Surface;

@SuppressLint("ViewConstructor")
class VideoSurfaceView extends GLSurfaceView {

    VideoRender mRenderer;
    private MediaPlayer mMediaPlayer = null;

    public VideoSurfaceView(Context context, MediaPlayer mp) {
        super(context);

        setEGLContextClientVersion(2);
        mMediaPlayer = mp;
        mRenderer = new VideoRender(context);
        setRenderer(mRenderer);
    }

    @Override
    public void onResume() {
        queueEvent(new Runnable(){
                public void run() {
                    mRenderer.setMediaPlayer(mMediaPlayer);
                }});

        super.onResume();
    }

    private static class VideoRender
        implements GLSurfaceView.Renderer, SurfaceTexture.OnFrameAvailableListener {
        private static String TAG = "VideoRender";
        
        private VideoView videoView = new VideoView();
       

        private SurfaceTexture mSurface;
        private boolean updateSurface = false;

       

        private MediaPlayer mMediaPlayer;

        public VideoRender(Context context) {
           
        }

        public void setMediaPlayer(MediaPlayer player) {
            mMediaPlayer = player;
        }
        
        @Override
        public void onDrawFrame(GL10 glUnused) {
            synchronized(this) {
                if (updateSurface) {
                    mSurface.updateTexImage();
                    mSurface.getTransformMatrix(videoView.getSTMatrix());
                    updateSurface = false;
                }
            }

            videoView.onDrawFrame();

        }

        @Override
        public void onSurfaceChanged(GL10 glUnused, int width, int height) {
        	
        }
        
        @Override
        public void onSurfaceCreated(GL10 glUnused, EGLConfig config) {
        		videoView.onSurfaceCreated();

            /*
             * Create the SurfaceTexture that will feed this textureID,
             * and pass it to the MediaPlayer
             */
            mSurface = new SurfaceTexture(videoView.getTextureID());
            mSurface.setOnFrameAvailableListener(this);

            Surface surface = new Surface(mSurface);
            mMediaPlayer.setSurface(surface);
            mMediaPlayer.setScreenOnWhilePlaying(true);
            surface.release();

            try {
                mMediaPlayer.prepare();
            } catch (IOException t) {
                Log.e(TAG, "media player prepare failed");
            }

            synchronized(this) {
                updateSurface = false;
            }
            
            mMediaPlayer.start();
        }

        synchronized public void onFrameAvailable(SurfaceTexture surface) {
            updateSurface = true;
        }

       

    }  // End of class VideoRender.

}  // End of class VideoSurfaceView.
