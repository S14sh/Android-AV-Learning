package base.mvp.view;

import android.content.Context;

public interface IBaseView {

    void showLoadingDialog();

    void dismissLoadingDialog();

    Context getContext();

    Class<?> getNextActivityClazz();

}
