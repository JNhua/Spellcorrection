#ifndef __WD_THREADPOOL_H__
#define __WD_THREADPOOL_H__

#include "TaskQueue.h"

#include <vector>
#include <memory>
#include <functional>

using namespace std;

namespace wd
{

class Thread;

class Threadpool
{
public:
	using Task = std::function<void()>;

	Threadpool(size_t threadNum, size_t queSize)
	: _threadNum(threadNum)
	, _queSize(queSize)
	, _taskQue(_queSize)
	, _isExit(false)
	{
		_threads.reserve(_threadNum);
	}

	~Threadpool();

	void start();
	void stop();
	void addTask(Task && task);

private:
	void threadFunc();
	Task getTask();

private:
	size_t _threadNum;
	size_t _queSize;
	vector<shared_ptr<Thread> > _threads;
	TaskQueue _taskQue;
	bool _isExit;
};


}//end of namespace wd

#endif
