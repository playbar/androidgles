package com.android.player;

import android.app.Activity;
import android.content.res.AssetFileDescriptor;
import android.content.res.Resources;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.util.Log;
import com.android.gltest.R;

public class MediaPlayerSurfaceStubActivity extends Activity {

	private static final String TAG = "MediaPlayerSurfaceStubActivity";

	protected Resources mResources;

	private VideoSurfaceView mVideoView = null;
	private MediaPlayer mMediaPlayer = null;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		mResources = getResources();
		mMediaPlayer = new MediaPlayer();

		try {
			AssetFileDescriptor afd = mResources.openRawResourceFd(R.raw.testvideo);
			mMediaPlayer.setDataSource(afd.getFileDescriptor(), afd.getStartOffset(), afd.getLength());
			afd.close();
//			mMediaPlayer.setDataSource("/sdcard/test.mp4");
		} catch (Exception ex) {
			Log.e(TAG, ex.getMessage(), ex);
//			Log.e(TAG, e.getMessage() );
		}

		mVideoView = new VideoSurfaceView(this, mMediaPlayer);
		setContentView(mVideoView);

	}

	@Override
	protected void onResume() {
		super.onResume();
		mVideoView.onResume();
	}
}
