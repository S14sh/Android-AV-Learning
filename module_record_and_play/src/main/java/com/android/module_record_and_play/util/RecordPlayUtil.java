package com.android.module_record_and_play.util;

import android.media.AudioAttributes;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.os.Build;
import android.util.Log;

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
        new Thread(() -> {
            try {
                if (mAudioTrack != null && mAudioTrack.getState() != AudioTrack.STATE_UNINITIALIZED && mAudioTrack.getPlayState() != AudioTrack.PLAYSTATE_PLAYING)
                    mAudioTrack.play();
            } catch (Exception e) {
                Log.e(TAG, "startPlay: 播放出错" + ' ' + e.getMessage() + e);
            }
        }).start();
        mStatus = STATUS_START;
    }

    public void stopPlay() {

    }
}
