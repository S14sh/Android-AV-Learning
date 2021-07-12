package util;

public enum FfmpegUtil {

    instance;

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-ffmpeg-lib");
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String getFfmpegVersion();

}
