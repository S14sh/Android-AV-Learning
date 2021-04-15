package com.android.module_common.application;

import android.app.Application;
import android.util.Log;

import base.application.ApplicationConfig;
import base.application.BaseApplication;

public class MainApplication extends BaseApplication {
    @Override
    public void initModuleApp(Application application) {
        for (String moduleApplication : ApplicationConfig.moduleApplications) {
            try {
                Class clazz = Class.forName(moduleApplication);
                BaseApplication baseApplication = (BaseApplication) clazz.newInstance();
                baseApplication.initModuleApp(this);
            } catch (Exception e) {
                Log.e("MainApplication", "initModuleApp: " + e.getMessage() + ' ' + e);
            }
        }
    }

    @Override
    public void initModuleData(Application application) {
        for (String moduleApplication : ApplicationConfig.moduleApplications) {
            try {
                Class clazz = Class.forName(moduleApplication);
                BaseApplication baseApplication = (BaseApplication) clazz.newInstance();
                baseApplication.initModuleData(this);
            } catch (Exception e) {
                Log.e("MainApplication", "initModuleData: " + e.getMessage() + ' ' + e);
            }
        }
    }
}
