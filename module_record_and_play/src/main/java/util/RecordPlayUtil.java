package util;

import android.content.Context;
import android.media.AudioAttributes;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.os.Build;
import android.text.TextUtils;
import android.util.Log;

import com.module_opensl_es.util.Opensl_esUtil;

import java.io.BufferedInputStream;
import java.io.DataInputStream;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

/**
 * 播放录音单例类
 */
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
    private Context mContext;
    /**
     * 单任务线程池
     */
    private ThreadPoolExecutor mExecutorService = new ThreadPoolExecutor(1, 1, 0L, TimeUnit.MILLISECONDS, new LinkedBlockingQueue<>());
    private StateChangeListener mStateChangeListener = null;

    /**
     * 初始化audioTrack（默认值）
     *
     * @param context
     */
    @SuppressWarnings("deprecation")
    public void createAudioTrack(Context context) {
        mContext = context;
        mBufferSize = AudioTrack.getMinBufferSize(SAMPLE_RATE, CHANNEL, AUDIO_FORMAT);
        if (mBufferSize <= 0) {
            Log.e(TAG, "createAudioTrack: " + "AudioTrack is not available because of mBufferSize is " + mBufferSize);
        }
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

    /**
     * 开始播放
     *
     * @param path
     */
    public void startAudioTrackPlay(String path) {
        if (TextUtils.isEmpty(path)) {
            Log.e(TAG, "startPlay: filePath is empty!!!");
            return;
        }
        mFilePath = path;
        if (mStatus == STATUS_NOT_READY || mAudioTrack == null) {
            Log.e(TAG, "startPlay: " + "播放器尚未初始化");
            return;
        } else if (mStatus == STATUS_START) {
            Log.e(TAG, "startPlay: " + "播放器正在播放...");
            return;
        }
        mExecutorService.execute(() -> playPCMFromFile());
        mStatus = STATUS_START;
    }

    /**
     * 从文件中播放pcm格式的数据
     */
    private void playPCMFromFile() {
        byte[] bytes = new byte[mBufferSize];
        int length = 0;
        InputStream is = null;
        try {
            is = new DataInputStream(new BufferedInputStream(new FileInputStream(mContext.getExternalCacheDir().getAbsolutePath() + mFilePath)));
            if (mAudioTrack != null && mAudioTrack.getState() != AudioTrack.STATE_UNINITIALIZED && mAudioTrack.getPlayState() != AudioTrack.PLAYSTATE_PLAYING) {
                mAudioTrack.play();
                if (mStateChangeListener != null) {
                    mStateChangeListener.OnPlayStart();
                }
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
    }

    /**
     * 停止播放
     */
    public void stopAudioTrackPlay() {
        if (mStatus == STATUS_READY || mStatus == STATUS_NOT_READY) {
            Log.e(TAG, "stopPlay: 播放器尚未播放");
        } else {
            mStatus = STATUS_STOP;
            mAudioTrack.stop();
            if (mStateChangeListener != null) {
                mStateChangeListener.OnPlayStop();
            }
            release();
        }
    }

    /**
     * 释放audioTrack
     */
    public void release() {
        mStatus = STATUS_NOT_READY;
        if (mAudioTrack != null) {
            mAudioTrack.release();
            mAudioTrack = null;
            mStateChangeListener = null;
        }
    }

    /**
     * 使用opensl_es播放PCM
     *
     * @param path
     */
    public void startOpenslPlay(String path) {
        Opensl_esUtil.instance.nativePlayPCM(path);
    }

    public void stopOpenslPlay() {
        Opensl_esUtil.instance.nativeStopPCM();
    }

    /**
     * 设置状态回调
     *
     * @param listener
     */
    void setStateChangeListener(StateChangeListener listener) {
        mStateChangeListener = listener;
    }

    /**
     * 开始播放和停止播放的回调
     */
    interface StateChangeListener {

        /**
         * 播放时回调
         */
        void OnPlayStart();

        /**
         * 停止播放时回调
         */
        void OnPlayStop();

    }

}
