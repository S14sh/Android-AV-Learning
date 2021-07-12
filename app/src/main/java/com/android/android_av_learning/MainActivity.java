package com.android.android_av_learning;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

import com.android.module_log.util.LogUtil;

import util.FfmpegUtil;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
//        tv.setText(FfmpegUtil.instance.getFfmpegVersion());
        LogUtil.instance.setLogLevel(LogUtil.LogLevel.VERBOSE);
        LogUtil.instance.d(null, "yes");
    }

}