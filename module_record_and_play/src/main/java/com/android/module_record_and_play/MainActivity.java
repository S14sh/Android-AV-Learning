package com.android.module_record_and_play;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.pm.PackageManager;
import android.os.Bundle;
import android.view.View;
import android.widget.Toast;

import com.android.module_record_and_play.databinding.ActivityMainBinding;

import util.RecordPlayUtil;
import util.RecordUtil;

/**
 * 该module下测试用activity
 */
public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    private ActivityMainBinding mViewBinding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mViewBinding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(mViewBinding.getRoot());
        mViewBinding.recordAndPlayStartRecord.setOnClickListener(this);
        mViewBinding.recordAndPlayStopRecord.setOnClickListener(this);
        mViewBinding.recordAndPlayStartAudioTrackPlay.setOnClickListener(this);
        mViewBinding.recordAndPlayStopAudioTrackPlay.setOnClickListener(this);
        mViewBinding.recordAndPlayStartOpenslPlay.setOnClickListener(this);
        mViewBinding.recordAndPlayStopOpenslPlay.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        if (v.getId() == R.id.record_and_play_startRecord) {
            RecordUtil.instance.createAudioRecord(this);
            RecordUtil.instance.startRecord("test1");
        } else if (v.getId() == R.id.record_and_play_stopRecord) {
            RecordUtil.instance.stopRecording();
        } else if (v.getId() == R.id.record_and_play_startAudioTrackPlay) {
            RecordPlayUtil.instance.createAudioTrack(this);
            RecordPlayUtil.instance.startAudioTrackPlay("test1");
        } else if (v.getId() == R.id.record_and_play_stopAudioTrackPlay) {
            RecordPlayUtil.instance.stopAudioTrackPlay();
        } else if (v.getId() == R.id.record_and_play_startOpenslPlay) {
            RecordPlayUtil.instance.startOpenslPlay("test1");
        } else if (v.getId() == R.id.record_and_play_stopOpenslPlay) {
            RecordPlayUtil.instance.stopOpenslPlay();
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