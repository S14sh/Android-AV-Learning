package com.android.module_fdkaac;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

import util.FdkAacUtil;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        TextView textView = findViewById(R.id.fdkaac_main_text);
        textView.setText(FdkAacUtil.getInstance().getFdkAacVersion());
    }
}