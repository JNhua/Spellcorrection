#include "TimerThread.h"


using namespace wd;

TimerThread::TimerThread(int initialTime, int intervalTime, TimerCallback && cb)
    : _timer(initialTime, intervalTime, std::move(cb))
    , _subThread(std::bind(&Timer::start, &_timer))
    , _isStarted(false)
{}

void TimerThread::start()
{
    _subThread.start();
    _isStarted = true;
}

void TimerThread::stop()
{
    if(_isStarted) {
        _timer.stop();
        _subThread.join();
        _isStarted = false;
    }
}

TimerThread::~TimerThread()
{
    if(_isStarted)
        stop();
}
