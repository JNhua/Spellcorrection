
#ifndef __WD_TIMER_H__
#define __WD_TIMER_H__

#include <functional>

namespace wd
{

class Timer
{
public:
	using TimerCallback = std::function<void()>;

	Timer(int initialTime, int intervalTime, TimerCallback && cb);

	~Timer();

	void start();
	void stop();
private:
	int createTimerFd();
	void setTimerfd(int initialTime, int intervalTime);
	void handleRead();
	
private:
	int _fd;
	int _initialTime;
	int _intervalTime;
	TimerCallback _cb;
	bool _isStarted;
};

}//end of namespace wd


#endif
