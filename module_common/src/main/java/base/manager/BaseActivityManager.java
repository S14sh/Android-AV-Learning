package base.manager;

import android.app.Activity;

import java.util.HashSet;
import java.util.Set;

public class BaseActivityManager {

    private Set<Activity> mActivity = new HashSet<>();

    public void addActivity(Activity activity) {
        if (mActivity != null)
            mActivity.add(activity);
    }

    public void removeActivity(Activity activity) {
        if (mActivity != null)
            mActivity.remove(activity);
    }

    public void clear() {
        mActivity.clear();
    }

}
