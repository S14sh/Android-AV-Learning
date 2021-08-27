package com.android.module_log.util;

/**
 * @author Slash.R.Wei
 * on 2021/8/24
 */
public enum NativeCatcherUtil {

    /**
     * 单例
     */
    instance;

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-log-lib");
    }

    public native void catchNativeException(CrashListener listener);

    public interface CrashListener {
        void onCrash(String threadName, Error error);
    }

}
