package util;

public class FfmpegUtil {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-ffmpeg-lib");
    }

    private static volatile FfmpegUtil instance;

    private FfmpegUtil(){

    }

    public static FfmpegUtil getInstance(){
        if(instance == null){
            synchronized (FfmpegUtil.class){
                if(instance == null)
                    instance = new FfmpegUtil();
            }
        }
        return instance;
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String getFfmpegVersion();

}
