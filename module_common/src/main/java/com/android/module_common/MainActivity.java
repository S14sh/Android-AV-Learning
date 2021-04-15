package com.android.module_common;

import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import androidx.lifecycle.ViewModelProvider;

import com.android.module_common.mvvm.MainViewModel;

import base.activity.BaseMvvmActivity;

public class MainActivity extends BaseMvvmActivity<MainViewModel>{

    private final String TAG = getClass().getSimpleName();
    private TextView mText;
    private EditText mEditor;
    private Button mBtn;

    @Override
    protected int getLayoutId() {
        return R.layout.activity_main;
    }

    @Override
    protected void initView() {
        mText = findViewById(R.id.common_main_text);
        mEditor = findViewById(R.id.common_main_editor);
        mBtn = findViewById(R.id.common_main_btn);
        mBtn.setOnClickListener((v) -> getViewModel().setText(mEditor.getText().toString()));
    }

    @Override
    protected void initData() {

    }

    @Override
    protected MainViewModel initViewModel() {
        return new ViewModelProvider(this).get(MainViewModel.class);
    }

    @Override
    protected void initObserves(MainViewModel viewModel) {
        viewModel.getText().observe(this, (text) -> mText.setText(text));
    }
}