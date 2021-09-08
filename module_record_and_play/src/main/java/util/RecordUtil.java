package util;

import android.content.Context;
import android.media.AudioFormat;
import android.media.AudioRecord;

import android.media.MediaRecorder;
import android.util.Log;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.concurrent.LinkedBlockingDeque;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

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
    /**
     * 单任务线程池
     */
    private ThreadPoolExecutor mExecutorService = new ThreadPoolExecutor(1, 1, 0L, TimeUnit.MILLISECONDS, new LinkedBlockingDeque<>());
    private StateChangeListener mStateChangeListener = null;

    /**
     * 初始化AudioRecord（默认值）
     *
     * @param context
     */
    public void createAudioRecord(Context context) {
        mContext = context;
        int bufferSize = AudioRecord.getMinBufferSize(FREQUENCY, AudioFormat.CHANNEL_IN_DEFAULT, AudioFormat.ENCODING_PCM_16BIT);
        createAudioRecord(context, MediaRecorder.AudioSource.MIC, FREQUENCY, AudioFormat.CHANNEL_IN_DEFAULT, AudioFormat.ENCODING_PCM_16BIT, bufferSize);
    }

    /**
     * 初始化AudioRecord（自定义）
     *
     * @param context       上下文，用于检查权限和获取写入文件的app的路径
     * @param audioSource   录音的输入源
     * @param sampleRate    采样率
     * @param channelConfig 声道信息
     * @param audioFormat   每个声道的数据信息
     * @param bufferSize    流中每次存储的buffer的大小
     */
    public void createAudioRecord(Context context, int audioSource, int sampleRate, int channelConfig, int audioFormat, int bufferSize) {
        if (mAudioRecord != null && isRecording()) {
            //如果已经在录音，直接退出
            Log.e(TAG, "startRecord: is RECORDING now!!!");
            return;
        }
        mRecordBuffSize = bufferSize;
        mAudioRecord = new AudioRecord(audioSource, sampleRate, channelConfig, audioFormat, bufferSize);
        if (!isAudioRecordInit()) {
            //初始化失败，直接退出
            Log.e(TAG, "createAudioRecord: audioRecord initialized failed!!!");
            return;
        }
        mData = new byte[mRecordBuffSize];
        mContext = context;
    }

    /**
     * 开始录音
     *
     * @param fileName
     */
    public void startRecord(String fileName) {
        if (!isAudioRecordInit()) {
            //初始化失败，直接退出
            Log.e(TAG, "startRecord: audioRecord initialized failed!!!");
            return;
        }
        if (isRecording()) {
            //已经在录音，先停止之前的录音
            Log.e(TAG, "startRecord: is RECORDING now!!!");
            stopRecording();
        }
        if (mStateChangeListener != null) {
            mStateChangeListener.OnRecordStart();
        }
        mExecutorService.execute(() -> {
            mAudioRecord.startRecording();
            write2File(fileName);
        });
    }

    /**
     * 判断是否正在录音
     *
     * @return
     */
    private boolean isRecording() {
        return mAudioRecord.getRecordingState() == AudioRecord.RECORDSTATE_RECORDING;
    }

    /**
     * 判断audioRecord是否初始化成功
     *
     * @return
     */
    private boolean isAudioRecordInit() {
        if (mAudioRecord == null || mAudioRecord.getState() == AudioRecord.STATE_UNINITIALIZED) {
            return false;
        }
        return true;
    }

    /**
     * 将录音数据写到指定文件中
     *
     * @param fileName
     */
    private void write2File(String fileName) {
        FileOutputStream fileOutputStream = null;
        int result = 0;
        try {
            fileOutputStream = new FileOutputStream(mContext.getExternalCacheDir().getAbsolutePath() + fileName);
        } catch (FileNotFoundException e) {
            Log.e(TAG, "write2File: file open error!!! " + e.getMessage() + ' ' + e);
        }
        if (null != fileOutputStream) {
            while (isRecording()) {
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

    /**
     * 停止录音
     */
    public void stopRecording() {
        if (isRecording()) {
            mAudioRecord.stop();
            if (mStateChangeListener != null) {
                mStateChangeListener.OnRecordStop();
            }
        }
        release();
    }

    /**
     * 释放audioRecord
     */
    public void release() {
        mAudioRecord.release();
        mAudioRecord = null;
        mStateChangeListener = null;
        mContext = null;
    }

    /**
     * 设置相应状态的回调
     *
     * @param listener
     */
    public void setStateChangeListener(StateChangeListener listener) {
        mStateChangeListener = listener;
    }

    /**
     * 开始录音和停止录音的回调
     */
    interface StateChangeListener {

        void OnRecordStart();

        void OnRecordStop();

    }

}
