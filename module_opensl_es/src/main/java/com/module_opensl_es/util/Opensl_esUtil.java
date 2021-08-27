package com.module_opensl_es.util;

/**
 * @author Slash.R.Wei
 * on 2021/8/27
 * opensl_es工具类
 */
public enum Opensl_esUtil {

    /**
     * 单例
     */
    instance;

    static {
        System.loadLibrary("native-opensl_es-lib");
    }

    public native void nativePlayPCM(String path);

    public native void nativeStopPCM();

}
