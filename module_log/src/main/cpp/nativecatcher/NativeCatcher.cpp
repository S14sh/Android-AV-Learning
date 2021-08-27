//
// Created by Slash.R.Wei on 2021/8/23.
//
#include "NativeCatcher.h"

const int NativeCatcher::exceptionSignals[] = {SIGSEGV, SIGABRT, SIGFPE, SIGILL, SIGBUS, SIGTRAP};

const int NativeCatcher::exceptionSignalNum =
        sizeof(exceptionSignals) / sizeof(exceptionSignals[0]);

struct sigaction NativeCatcher::oldHandlers[];

void NativeCatcher::signalPass(int code, siginfo_t *si, void *sc) {
    LOGE("监听到native异常！");
    //防止非信号方式导致死锁
    signal(code, SIG_DFL);
    signal(SIGALRM, SIG_DFL);
    (void) alarm(8);
    //解析栈信息，回调给java层，上报到后台或者保存本地文件
    notifyCaughtSignal(code, si, sc);
    //给系统原来默认的处理，否则会进入死循环
    oldHandlers[code].sa_sigaction(code, si, sc);
}

void NativeCatcher::notifyCaughtSignal(int code, siginfo_t *si, void *sc) {

}

/**
 * 安装信号捕获到native crash
 * @return
 */
bool NativeCatcher::installSignalHandlers() {
    //保存原来的信号处理
    for (int i = 0; i < exceptionSignalNum; ++i) {
        // int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact));
        // signum：代表信号编码，可以是除SIGKILL及SIGSTOP外的任何一个特定有效的信号，如果为这两个信号定义自己的处理函数，将导致信号安装错误。
        // act：指向结构体sigaction的一个实例的指针，该实例指定了对特定信号的处理，如果设置为空，进程会执行默认处理。
        // oldact：和参数act类似，只不过保存的是原来对相应信号的处理，也可设置为NULL。
        if (sigaction(exceptionSignals[i], nullptr, &oldHandlers[exceptionSignals[i]]) == -1) {
            return false;
        }
    }
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    //不同堆栈处理并且可将参数传递下去
    sa.sa_flags = SA_ONSTACK | SA_SIGINFO;
    // 指定信号处理的回调函数
    sa.sa_sigaction = signalPass;
    //处理当前信号量的时候不考虑其他的
    for (int i = 0; i < exceptionSignalNum; ++i) {
        //阻塞其他信号的
        sigaddset(&sa.sa_mask, exceptionSignals[i]);
    }
    for (int i = 0; i < exceptionSignalNum; ++i) {
        //处理自己的信号，如果成功返回0，失败返回-1
        if (sigaction(exceptionSignals[i], &sa, NULL) == -1) {
            LOGE("信号量处理失败！");
        }
    }
    return true;
}

/**
 * 防止因栈溢出导致中断，而导致在默认栈上运行可能破坏程序运行的现场，无法获取到正确的上下文。
 * 开辟一块新的空间作为运行信号处理函数的栈。使用signalStack在任意线程注册一个可选的栈，保留一下在紧急情况下使用的空间。
 * @param __new_signal_stack
 * @param __old_signal_stack
 * @return
 */
int NativeCatcher::signalStack(const stack_t *__new_signal_stack, stack_t *__old_signal_stack) {
    stack_t newStack;
    stack_t oldStack;
    memset(&newStack, 0, sizeof(newStack));
    memset(&oldStack, 0, sizeof(oldStack));
    static const unsigned signalStackSize = std::max(16384, SIGSTKSZ);
    if (sigaltstack(nullptr, &oldStack) == -1
        || !oldStack.ss_sp
        || oldStack.ss_size < signalStackSize) {
        newStack.ss_sp = calloc(1, signalStackSize);
        newStack.ss_size = signalStackSize;
        if (sigaltstack(&newStack, NULL) == -1) {
            free(newStack.ss_sp);
        }
    }
}
