#include "Thread.h"
#include <iostream>

using std::cout;
using std::endl;

namespace wd {
__thread int thread_index=0;
//右值引用本身到底是左值还是右值，取决于其是否有名字
Thread::Thread(ThreadCallback && cb,int index)
    : _pthid(0)
    , _isRunning(false)
    , _cb(std::move(cb))
    ,_index(index)
{}

Thread::Thread(ThreadCallback && cb)
    : _pthid(0)
    , _isRunning(false)
    , _cb(std::move(cb))
{}

void Thread::start()
{
    pthread_create(&_pthid, NULL, threadFunc, this);
    _isRunning = true;
}

void * Thread::threadFunc(void * arg)
{
    Thread * pthread = static_cast<Thread*>(arg);
    if(pthread)
    {
        wd::thread_index=pthread->_index; //在线程里才能初始化index
        pthread->_cb();// 执行任务
    }
    return NULL;
}

void Thread::join()
{
    pthread_join(_pthid, NULL);
    _isRunning = false;
}

Thread::~Thread()
{
    if(_isRunning)
    {
        pthread_detach(_pthid);// 将运行的线程交给系统进行托管
        _isRunning = false;
    }
    cout << "~Thread()" << endl;
}
}
