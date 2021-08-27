//
// Created by Slash.R.Wei on 2021/8/23.
// 用于捕获native中断信号量
//

#ifndef ANDROID_AV_LEARNING_NATIVECATCHER_H
#define ANDROID_AV_LEARNING_NATIVECATCHER_H

#include <signal.h>
#include <iostream>
#include <android/log.h>
#include <zconf.h>
#include <memory>
#include <unwind.h>

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __ANDROID__
#define LOG_TAG "NativeCatcher"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#endif

//进程发现错误或者调用了abort()
//#define SIGABRT 6
//不存在的物理地址，硬件有误
//#define SIGBUS 7, 10
//浮点数运算错误(如除0，余0，整形溢出)
//#define SIGFPE 8
//非法指令(损坏的可执行文件或者代码区损坏)
//#define SIGILL 4
//段地址错误(空指针，访问不存在的地址空间，访问内核区，写只读空间，栈溢出，数组越界，野指针)
//#define SIGSEGV 11
//管道错误，往没有reader的管道中写(linux中的socket,如果断掉了还继续写。signal(SIGPIPE, SIG_IGN))
//#define SIGPIPE 13

class NativeCatcher {
private:
    //异常信号量
    const static int exceptionSignals[];
    //异常信号量个数
    const static int exceptionSignalNum;
    //旧信号量
    static struct sigaction oldHandlers[NSIG];
public:
    //构造函数
    NativeCatcher() {}

    //析构函数
    virtual ~NativeCatcher() {}

    static void signalPass(int code, siginfo_t *si, void *sc);

    static void notifyCaughtSignal(int code, siginfo_t *si, void *sc);

    bool installSignalHandlers();

    int signalStack(const stack_t* __new_signal_stack, stack_t* __old_signal_stack);

    _Unwind_Reason_Code unwind_callback(struct _Unwind_Context *context, void *arg);

};

#ifdef __cplusplus
}
#endif
#endif //ANDROID_AV_LEARNING_NATIVECATCHER_H
