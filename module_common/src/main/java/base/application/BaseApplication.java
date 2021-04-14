package base.application;

import android.app.Application;

import base.manager.BaseActivityManager;

public abstract class BaseApplication extends Application {

    protected static BaseApplication instance;

    protected BaseActivityManager mBaseActivityManager;

    public static BaseApplication getInstance() {
        return instance;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        instance = this;
        initModuleApp(this);
        initModuleData(this);
    }

    /**
     * Application初始化
     *
     * @param application
     */
    public abstract void initModuleApp(Application application);

    /**
     * 所有application初始化后的自定义操作
     *
     * @param application
     */
    public abstract void initModuleData(Application application);

    public BaseActivityManager getActivityManager() {
        if (mBaseActivityManager == null)
            mBaseActivityManager = new BaseActivityManager();
        return mBaseActivityManager;
    }

}
