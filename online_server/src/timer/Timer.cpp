#include "Timer.h"

#include <unistd.h>
#include <errno.h>
#include <poll.h>
#include <sys/timerfd.h>

#include <iostream>
using namespace std; 
using namespace wd;





Timer::Timer(int initialTime, int intervalTime, TimerCallback && cb)
: _fd(createTimerFd())
, _initialTime(initialTime)
, _intervalTime(intervalTime)
, _cb(std::move(cb))
, _isStarted(false)
{}

void Timer::start() 
{
	struct pollfd pfd;
	pfd.fd = _fd;
	pfd.events = POLLIN;

	setTimerfd(_initialTime, _intervalTime);//开启定时器

	_isStarted = true;
	while(_isStarted) {
		int nready = ::poll(&pfd, 1, 5000);
		if(nready == - 1 && errno == EINTR) {
			continue;
		} else if(nready == -1) {
			perror(">>> poll error");
			exit(EXIT_FAILURE);
		} else if(nready == 0) {
			cout << ">>> poll timeout!" << endl;
		} else {
			if(pfd.revents & POLLIN) {
				handleRead();//先处理定时器
				if(_cb)
					_cb(); //再去执行任务
			}
		}
	}
}

void Timer::stop()
{
	setTimerfd(0, 0);//停止定时器
	if(_isStarted)
		_isStarted = false;
}

Timer::~Timer()
{
	if(_isStarted)
		stop();
}

int Timer::createTimerFd()
{
	int fd = ::timerfd_create(CLOCK_REALTIME,0);
	if(fd == -1) {
		perror(">> timerfd_create error");
	}
	return fd;
}


void Timer::setTimerfd(int initialTime, int intervalTime)
{
	struct itimerspec value;
	value.it_value.tv_sec = initialTime;
	value.it_value.tv_nsec = 0;
	value.it_interval.tv_sec = intervalTime;
	value.it_interval.tv_nsec = 0;
	int ret = ::timerfd_settime(_fd, 0, &value, NULL);
	if(ret == -1) {
		perror(">>> timerfd_settime error");
	}
}

void Timer::handleRead()
{
	uint64_t howmany;
	int ret = ::read(_fd, &howmany, sizeof(uint64_t));
	if(ret != sizeof(uint64_t)) {
		perror(">>> read error");
	}
}
