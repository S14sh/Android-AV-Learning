package base.activity;

import android.content.Context;
import android.os.Bundle;

import androidx.annotation.Nullable;

import base.dialogfragment.LoadingDialog;
import base.mvp.presenter.BasePresenter;
import base.mvp.view.IBaseView;

public abstract class BaseMvpActivity<P extends BasePresenter>
        extends BaseActivity
        implements IBaseView {

    protected P mPresenter;
    private LoadingDialog mLoadingDialog;
    private final String TAG = getClass().getSimpleName();

    protected abstract P createPresenter();

    protected P getPresenter() {
        return mPresenter;
    }

    protected abstract void initData();

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        mPresenter = createPresenter();
        if (mPresenter != null)
            mPresenter.attachView(this);
        super.onCreate(savedInstanceState);
        initData();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (mPresenter != null) {
            mPresenter.detachView();
            mPresenter = null;
        }
    }

    @Override
    public void showLoadingDialog() {
        if (mLoadingDialog == null) {
            mLoadingDialog = new LoadingDialog();
        }
        mLoadingDialog.show(getSupportFragmentManager().beginTransaction(), TAG);
    }

    @Override
    public void dismissLoadingDialog() {
        if (mLoadingDialog != null)
            mLoadingDialog.dismiss();
    }

    @Override
    public Context getContext() {
        return this;
    }
}
