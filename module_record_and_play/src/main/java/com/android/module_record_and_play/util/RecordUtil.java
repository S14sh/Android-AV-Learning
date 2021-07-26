package com.android.module_record_and_play.util;

import android.content.Context;
import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;
import android.util.Log;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

//录音单例类
public enum RecordUtil {

    instance;

    private final String TAG = getClass().getSimpleName();
    private final int FREQUENCY = 44100;

    private AudioRecord mAudioRecord = null;
    //用于申请权限相关
    private Context mContext = null;
    private int mRecordBuffSize = 0;
    private byte[] mData;
    private volatile boolean mIsRecording = false;
    // 单任务线程池
    private ExecutorService mExecutorService = Executors.newSingleThreadExecutor();
    private StateChangeListener mStateChangeListener = null;

    public void createAudioRecord(Context context) {
        int bufferSize = AudioRecord.getMinBufferSize(FREQUENCY, AudioFormat.CHANNEL_IN_DEFAULT, AudioFormat.ENCODING_PCM_16BIT);
        createAudioRecord(context, MediaRecorder.AudioSource.MIC, FREQUENCY, AudioFormat.CHANNEL_IN_DEFAULT, AudioFormat.ENCODING_PCM_16BIT, bufferSize);
    }

    public void createAudioRecord(Context context, int audioSource, int sampleRate, int channelConfig, int audioFormat, int bufferSize) {
        if (mIsRecording) {
            Log.e(TAG, "startRecord: is RECORDING now!!!");
            return;
        }
        mRecordBuffSize = bufferSize;
        mAudioRecord = new AudioRecord(audioSource, sampleRate, channelConfig, audioFormat, bufferSize);
        if (mAudioRecord.getState() == AudioRecord.STATE_UNINITIALIZED) {
            Log.e(TAG, "createAudioRecord: audioRecord initialized failed!!!");
            return;
        }
        mData = new byte[mRecordBuffSize];
        mContext = context;
    }

    public void startRecord(String fileName) {
        if (mIsRecording) {
            Log.e(TAG, "startRecord: is RECORDING now!!!");
            return;
        }
        if (mAudioRecord.getState() == AudioRecord.STATE_UNINITIALIZED) {
            Log.e(TAG, "startRecord: audioRecord initialized failed!!!");
            return;
        }
        mAudioRecord.startRecording();
        if (mStateChangeListener != null)
            mStateChangeListener.OnRecordStart();
        mIsRecording = true;
        mExecutorService.execute(() -> write2File(fileName));
    }

    private void write2File(String fileName) {
        FileOutputStream fileOutputStream = null;
        int result = 0;
        try {
            fileOutputStream = new FileOutputStream(fileName);
        } catch (FileNotFoundException e) {
            Log.e(TAG, "write2File: file open error!!! " + e.getMessage() + ' ' + e);
        }
        if (null != fileOutputStream) {
            while (mIsRecording) {
                result = mAudioRecord.read(mData, 0, mRecordBuffSize);
                if (AudioRecord.ERROR_INVALID_OPERATION != result) {
                    try {
                        fileOutputStream.write(mData);
                    } catch (IOException e) {
                        Log.e(TAG, "write2File: write file error!!! " + e.getMessage() + ' ' + e);
                    }
                }
            }
            try {
                fileOutputStream.close();
            } catch (IOException e) {
                Log.e(TAG, "write2File: file close error!!! " + e.getMessage() + ' ' + e);
            }
        }
    }

    public void stopRecording() {
        Log.e(TAG, "stopRecording: audioStop");
        mIsRecording = false;
        if (mAudioRecord.getState() == AudioRecord.RECORDSTATE_RECORDING) {
            mAudioRecord.stop();
            if (mStateChangeListener != null)
                mStateChangeListener.OnRecordStop();
        }
        release();
    }

    public void release() {
        Log.e(TAG, "release: audioRecord release");
        mAudioRecord.release();
        mAudioRecord = null;
        mStateChangeListener = null;
        mContext = null;
    }

    public void setStateChangeListener(StateChangeListener listener) {
        mStateChangeListener = listener;
    }

    interface StateChangeListener {

        void OnRecordStart();

        void OnRecordStop();

    }

}
