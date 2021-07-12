package com.android.module_log.util;

import android.text.TextUtils;
import android.util.Log;

public enum LogUtil {

    instance;

    public enum LogLevel {
        VERBOSE,
        DEBUG,
        INFO,
        WARN,
        ERROR;
    }

    private LogLevel level = LogLevel.VERBOSE;

    public void setLogLevel(LogLevel l) {
        level = l;
    }

    public void v(String tag, String info) {
        if (level.compareTo(LogLevel.VERBOSE) <= 0) {
            if (TextUtils.isEmpty(tag))
                tag = getClass().getSimpleName();
            Log.v(tag, info);
        }
    }

    public void d(String tag, String info) {
        if (level.compareTo(LogLevel.DEBUG) <= 0) {
            if (TextUtils.isEmpty(tag))
                tag = getClass().getSimpleName();
            Log.d(tag, info);
        }
    }

    public void i(String tag, String info) {
        if (level.compareTo(LogLevel.INFO) <= 0) {
            if (TextUtils.isEmpty(tag))
                tag = getClass().getSimpleName();
            Log.i(tag, info);
        }
    }

    public void w(String tag, String info) {
        if (level.compareTo(LogLevel.WARN) <= 0)
            if (TextUtils.isEmpty(tag))
                tag = getClass().getSimpleName();
        Log.w(tag, info);
    }

    public void e(String tag, String info) {
        if (level.compareTo(LogLevel.ERROR) <= 0) {
            if (TextUtils.isEmpty(tag))
                tag = getClass().getSimpleName();
            Log.e(tag, info);
        }
    }

}
