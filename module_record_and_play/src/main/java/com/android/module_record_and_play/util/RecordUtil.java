package com.android.module_record_and_play.util;

import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

public enum RecordUtil {

    instance;

    private final int FREQUENCY = 44100;

    private AudioRecord mAudioRecord = null;
    private int mRecordBuffSize = 0;
    private byte[] mData;
    private boolean mIsRecording = false;

    public void createAudioRecord() {
        mRecordBuffSize = AudioRecord.getMinBufferSize(FREQUENCY, AudioFormat.CHANNEL_IN_DEFAULT, AudioFormat.ENCODING_PCM_16BIT);
        mAudioRecord = new AudioRecord(MediaRecorder.AudioSource.MIC, FREQUENCY, AudioFormat.CHANNEL_IN_DEFAULT, AudioFormat.ENCODING_PCM_16BIT, mRecordBuffSize);
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
            e.printStackTrace();
        }
        if (null != fileOutputStream) {
            while (mIsRecording) {
                result = mAudioRecord.read(mData, 0, mRecordBuffSize);
                if (AudioRecord.ERROR_INVALID_OPERATION != result) {
                    try {
                        fileOutputStream.write(mData);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
            try {
                fileOutputStream.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public void stopRecording() {
        mIsRecording = false;
    }

    public void destroy() {

    }

}
