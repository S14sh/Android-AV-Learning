package com.android.module_record_and_play;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.pm.PackageManager;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import com.android.module_record_and_play.util.RecordPlayUtil;
import com.android.module_record_and_play.util.RecordUtil;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    private Button mBtnStartRecord, mBtnStopRecord;
    private Button mBtnStartPlay, mBtnStopPlay;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mBtnStartRecord = findViewById(R.id.record_and_play_startRecord);
        mBtnStartRecord.setOnClickListener(this);
        mBtnStopRecord = findViewById(R.id.record_and_play_stopRecord);
        mBtnStopRecord.setOnClickListener(this);
        mBtnStartPlay = findViewById(R.id.record_and_play_startPlay);
        mBtnStartPlay.setOnClickListener(this);
        mBtnStopPlay = findViewById(R.id.record_and_play_stopPlay);
        mBtnStopPlay.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        if (v.getId() == R.id.record_and_play_startRecord) {
            RecordUtil.instance.createAudioRecord(this);
            RecordUtil.instance.startRecord("test1");
        } else if (v.getId() == R.id.record_and_play_stopRecord) {
            RecordUtil.instance.stopRecording();
        } else if (v.getId() == R.id.record_and_play_startPlay) {
            RecordPlayUtil.instance.createAudioTrack(this);
            RecordPlayUtil.instance.startPlay("test1");
        } else if (v.getId() == R.id.record_and_play_stopPlay) {
            RecordPlayUtil.instance.stopPlay();
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == 0) {
            for (int result : grantResults) {
                if (result != PackageManager.PERMISSION_GRANTED) {
                    Toast.makeText(this, "未获得必要权限，应用无法正常运行！", Toast.LENGTH_SHORT).show();
                    finish();
                }
            }
        }
    }
}