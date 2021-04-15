package com.android.module_common.mvvm;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;

import base.mvvm.viewmodel.BaseViewModel;

public class MainViewModel extends BaseViewModel {

    private MutableLiveData<String> mText;

    public MainViewModel() {
        mText = new MutableLiveData<>();
    }

    public LiveData<String> getText(){
        return mText;
    }

    public void setText(String text){
        mText.setValue(text);
    }

}
