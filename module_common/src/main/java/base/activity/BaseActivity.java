package base.activity;

import android.content.Intent;
import android.os.Bundle;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import base.application.BaseApplication;

public abstract class BaseActivity extends AppCompatActivity {

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(getLayoutId());
        initView();
        BaseApplication.getInstance().getActivityManager().addActivity(this);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        BaseApplication.getInstance().getActivityManager().removeActivity(this);
    }

    protected abstract int getLayoutId();

    protected abstract void initView();

    public void jumpToActivity(Class<?> clazz) {
        Intent intent = new Intent(this, clazz);
        jumpToActivity(intent);
    }

    public void jumpToActivity(Intent intent) {
        startActivity(intent);
    }



}
