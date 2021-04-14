package com.android.module_common;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import base.dialogfragment.LoadingDialog;

public class MainActivity extends AppCompatActivity {

    private final String TAG = getClass().getSimpleName();
    private LoadingDialog mLoadingDialog = new LoadingDialog();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mLoadingDialog.show(getSupportFragmentManager().beginTransaction(), TAG);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mLoadingDialog.dismiss();
    }
}