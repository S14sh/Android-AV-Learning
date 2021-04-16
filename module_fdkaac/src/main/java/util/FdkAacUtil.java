package util;

public class FdkAacUtil {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-fdkaac-lib");
    }

    private static volatile FdkAacUtil instance;

    private FdkAacUtil() {

    }

    public static FdkAacUtil getInstance() {
        if (instance == null) {
            synchronized (FdkAacUtil.class) {
                if (instance == null)
                    instance = new FdkAacUtil();
            }
        }
        return instance;
    }

    public native String getFdkAacVersion();

}
