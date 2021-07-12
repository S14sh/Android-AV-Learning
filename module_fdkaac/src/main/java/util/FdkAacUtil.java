package util;

public enum FdkAacUtil {

    instance;

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-fdkaac-lib");
    }

    public native String getFdkAacVersion();

}
