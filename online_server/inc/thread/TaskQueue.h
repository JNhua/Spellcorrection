#ifndef __WD_TASKQUEUE_H__
#define __WD_TASKQUEUE_H__

#include "MutexLock.h"
#include "Condition.h"

#include <queue>
#include <functional>


namespace wd
{

typedef std::function<void()> Task;

class TaskQueue
{
public:
	TaskQueue(size_t queSize)
	: _queSize(queSize)
	, _mutex()
	, _notFull(_mutex)
	, _notEmpty(_mutex)
	, _flag(true)
	{}
	
	void push(Task && task);
	Task pop();

	bool empty() const
	{	return _que.size() == 0;	}

	bool full()const
	{	return _que.size() == _queSize;	}

	void wakeup() {
		_flag = false;
		_notEmpty.notifyAll();
	}

private:
	size_t _queSize;
	std::queue<Task> _que;
	MutexLock _mutex;
	Condition _notFull;
	Condition _notEmpty;
	bool _flag;
};
}//end of namespace wd

#endif
