package base.dialogfragment;

import android.os.Bundle;

import androidx.annotation.Nullable;

import base.mvp.presenter.BasePresenter;
import base.mvp.view.IBaseView;

public abstract class BaseMvpDialogFragment<P extends BasePresenter>
        extends BaseDialogFragment
        implements IBaseView {

    protected P mPresenter;

    protected abstract P createPresenter();

    protected P getPresenter() {
        return mPresenter;
    }

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        mPresenter = createPresenter();
        if (mPresenter != null)
            mPresenter.attachView(this);
        super.onCreate(savedInstanceState);
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        if (mPresenter != null) {
            mPresenter.detachView();
            mPresenter = null;
        }
    }
}
