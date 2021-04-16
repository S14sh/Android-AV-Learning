package base.activity;

import android.os.Bundle;

import androidx.annotation.Nullable;

import base.mvvm.viewmodel.BaseViewModel;

public abstract class BaseMvvmActivity<VM extends BaseViewModel>
        extends BaseActivity {

    private VM mViewModel;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mViewModel = initViewModel();
        initObserves(mViewModel);
        initData();
    }

    protected abstract void initData();

    protected abstract VM initViewModel();

    protected abstract void initObserves(VM viewModel);

    protected VM getViewModel() {
        return mViewModel;
    }

}
