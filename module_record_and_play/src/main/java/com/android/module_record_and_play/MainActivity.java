package com.android.module_record_and_play;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    private Button mBtnStart, mBtnStop;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mBtnStart = findViewById(R.id.record_and_play_startRecord);
        mBtnStart = findViewById(R.id.record_and_play_stopRecord);
    }

    @Override
    public void onClick(View v) {
        if (v.getId() == R.id.record_and_play_startRecord) {

        } else if (v.getId() == R.id.record_and_play_stopRecord) {

        }
    }
}