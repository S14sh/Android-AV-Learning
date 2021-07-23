package com.android.module_record_and_play.util;

import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;
import android.util.Log;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

public enum RecordUtil {

    instance;

    private final String TAG = getClass().getSimpleName();
    private final int FREQUENCY = 44100;

    private AudioRecord mAudioRecord = null;
    private int mRecordBuffSize = 0;
    private byte[] mData;
    private boolean mIsRecording = false;

    public void createAudioRecord() {
        int bufferSize = AudioRecord.getMinBufferSize(FREQUENCY, AudioFormat.CHANNEL_IN_DEFAULT, AudioFormat.ENCODING_PCM_16BIT);
        createAudioRecord(MediaRecorder.AudioSource.MIC, FREQUENCY, AudioFormat.CHANNEL_IN_DEFAULT, AudioFormat.ENCODING_PCM_16BIT, bufferSize);
    }

    public void createAudioRecord(int audioSource, int sampleRate, int channelConfig, int audioFormat, int bufferSize) {
        mRecordBuffSize = bufferSize;
        mAudioRecord = new AudioRecord(audioSource, sampleRate, channelConfig, audioFormat, bufferSize);
        mData = new byte[mRecordBuffSize];
    }

    public void startRecord(String fileName) {
        mAudioRecord.startRecording();
        mIsRecording = true;
        write2File(fileName);
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
        mIsRecording = false;
    }

    public void destroy() {

    }

}
