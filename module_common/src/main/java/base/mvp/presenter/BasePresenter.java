package base.mvp.presenter;

import android.content.Context;

import java.lang.ref.WeakReference;
import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;

import base.mvp.model.IBaseModel;
import base.mvp.view.IBaseView;

public abstract class BasePresenter<V extends IBaseView, M extends IBaseModel> {

    protected V mProxyView;
    protected M mModel;
    protected WeakReference<V> mWeakReference;

    public void attachView(V view) {
        mWeakReference = new WeakReference<>(view);
        mProxyView = (V) Proxy.newProxyInstance(view.getClass().getClassLoader(), view.getClass().getInterfaces(), new MvpViewHandler(mWeakReference.get()));
        if (this.mModel == null)
            this.mModel = createModel();
    }

    public void detachView() {
        this.mModel = null;
        if (isViewAttached()) {
            mWeakReference.clear();
            mWeakReference = null;
        }
    }

    protected V getView() {
        return mProxyView;
    }

    protected abstract M createModel();

    protected M getModel() {
        return this.mModel;
    }

    protected Context getContext() {
        return getView().getContext();
    }

    protected boolean isViewAttached() {
        return mWeakReference != null && mWeakReference.get() != null;
    }

    protected class MvpViewHandler implements InvocationHandler {

        private V mvpView;

        MvpViewHandler(V mvpView) {
            this.mvpView = mvpView;
        }

        @Override
        public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
            if (isViewAttached())
                return method.invoke(mvpView, args);
            return null;
        }
    }

}
