package com.android.module_ffmpeg;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

import util.FfmpegUtil;

/**
 * 测试用activity
 */
public class MainActivity extends AppCompatActivity {


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        TextView textView = findViewById(R.id.ffmpeg_main_text);
        textView.setText(FfmpegUtil.instance.getFfmpegVersion());
    }


}