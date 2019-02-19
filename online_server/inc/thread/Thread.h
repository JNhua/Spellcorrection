#ifndef __WD_THREAD_H__
#define __WD_THREAD_H__

#include "Noncopyable.h"

#include <pthread.h>
#include <functional>
using std::function;

namespace wd
{
extern __thread int thread_index;
class Thread
        : Noncopyable // 表达语义
{
    using ThreadCallback = function<void()>;
public:
    Thread(ThreadCallback && cb,int);
    Thread(ThreadCallback && cb);
    ~Thread();

    void start();
    void join();

    bool isRunning() const {	return _isRunning;	}

private:
    static void * threadFunc(void *);//采用静态成员函数，就是为了去掉this的影响
private:
    pthread_t _pthid;
    bool _isRunning;
    ThreadCallback _cb;
    int _index;
};
}//end of namespace wd


#endif
