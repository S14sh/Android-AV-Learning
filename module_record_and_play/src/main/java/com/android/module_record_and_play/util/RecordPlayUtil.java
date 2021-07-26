package com.android.module_record_and_play.util;

import android.media.AudioAttributes;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.os.Build;
import android.util.Log;

import java.io.BufferedInputStream;
import java.io.DataInputStream;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

//播放录音单例类
public enum RecordPlayUtil {

    instance;

    private final String TAG = getClass().getSimpleName();
    private final int STATUS_READY = 0;
    private final int STATUS_NOT_READY = 1;
    private final int STATUS_START = 2;
    private final int STATUS_STOP = 3;
    private final int SAMPLE_RATE = 44100;
    private final int CHANNEL = AudioFormat.CHANNEL_OUT_MONO;
    private final int AUDIO_FORMAT = AudioFormat.ENCODING_PCM_16BIT;

    private int mBufferSize = 0;
    private AudioTrack mAudioTrack;
    private volatile int mStatus = STATUS_NOT_READY;
    private String mFilePath;
    // 单任务线程池
    private ExecutorService mExecutorService = Executors.newSingleThreadExecutor();
    private StateChangeListener mStateChangeListener = null;

    public void createAudioTrack(String path) {
        mFilePath = path;
        mBufferSize = AudioTrack.getMinBufferSize(SAMPLE_RATE, CHANNEL, AUDIO_FORMAT);
        if (mBufferSize <= 0)
            Log.e(TAG, "createAudioTrack: " + "AudioTrack is not available because of bufferSize is " + mBufferSize);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            mAudioTrack = new AudioTrack.Builder()
                    .setAudioAttributes(new AudioAttributes.Builder()
                            .setUsage(AudioAttributes.USAGE_MEDIA)
                            .setContentType(AudioAttributes.CONTENT_TYPE_MUSIC)
                            .setLegacyStreamType(AudioManager.STREAM_MUSIC)
                            .build())
                    .setAudioFormat(new AudioFormat.Builder()
                            .setEncoding(AUDIO_FORMAT)
                            .setSampleRate(SAMPLE_RATE)
                            .setChannelMask(CHANNEL)
                            .build())
                    .setTransferMode(AudioTrack.MODE_STREAM)
                    .setBufferSizeInBytes(mBufferSize)
                    .build();
        } else {
            mAudioTrack = new AudioTrack(AudioManager.STREAM_MUSIC, SAMPLE_RATE, CHANNEL, AUDIO_FORMAT, mBufferSize, AudioTrack.MODE_STREAM);
        }
        mStatus = STATUS_READY;
    }

    public void startPlay() {
        if (mStatus == STATUS_NOT_READY || mAudioTrack == null) {
            Log.e(TAG, "startPlay: " + "播放器尚未初始化");
            return;
        } else if (mStatus == STATUS_START) {
            Log.e(TAG, "startPlay: " + "播放器正在播放...");
            return;
        }
        mExecutorService.execute(() -> {
            byte[] bytes = new byte[mBufferSize];
            int length = 0;
            InputStream is = null;
            try {
                is = new DataInputStream(new BufferedInputStream(new FileInputStream(mFilePath)));
                if (mAudioTrack != null && mAudioTrack.getState() != AudioTrack.STATE_UNINITIALIZED && mAudioTrack.getPlayState() != AudioTrack.PLAYSTATE_PLAYING) {
                    Log.e(TAG, "startPlay: 开始播放");
                    mAudioTrack.play();
                    if (mStateChangeListener != null)
                        mStateChangeListener.OnPlayStart();
                }
                while ((length = is.read(bytes)) != -1 && mStatus == STATUS_START) {
                    //write方法会阻塞
                    mAudioTrack.write(bytes, 0, length);
                }
                Log.e(TAG, "startPlay: 播放结束！");
            } catch (Exception e) {
                Log.e(TAG, "startPlay: 播放出错" + ' ' + e.getMessage() + e);
            } finally {
                if (is != null) {
                    try {
                        is.close();
                    } catch (IOException e) {
                        Log.e(TAG, "startPlay: inputStream close error!!!");
                    }
                }
            }
        });
        mStatus = STATUS_START;
    }

    public void stopPlay() {
        if (mStatus == STATUS_READY || mStatus == STATUS_NOT_READY)
            Log.e(TAG, "stopPlay: 播放器尚未播放");
        else {
            mStatus = STATUS_STOP;
            mAudioTrack.stop();
            if (mStateChangeListener != null)
                mStateChangeListener.OnPlayStop();
            release();
        }
    }

    public void release() {
        mStatus = STATUS_NOT_READY;
        if (mAudioTrack != null) {
            mAudioTrack.release();
            mAudioTrack = null;
            mStateChangeListener = null;
        }
    }

    void setStateChangeListener(StateChangeListener listener) {
        mStateChangeListener = listener;
    }

    interface StateChangeListener {

        void OnPlayStart();

        void OnPlayStop();

    }

}
