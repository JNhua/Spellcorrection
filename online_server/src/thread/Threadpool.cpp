#include "Threadpool.h"
#include "Thread.h"
#include <unistd.h>
#include <iostream>

using namespace std;
using namespace wd;


void Threadpool::start()
{
	for(size_t idx = 0; idx < _threadNum; ++idx)
	{
        shared_ptr<Thread> pThread(new Thread(std::bind(&Threadpool::threadFunc, this),idx));
		_threads.push_back(std::move(pThread));
	}

	for(auto & pThread : _threads)
	{
		pThread->start();
	}
}

//运行在主线程中
void Threadpool::stop()
{
	if(!_isExit) {
		while(!_taskQue.empty()) {
			::sleep(1);
			cout << ">>> Threadpool sleep 1 s" << endl;
		}

		_isExit = true;
		cout << ">>> Threadpool -> stop: _isExit = " << _isExit << endl;

		_taskQue.wakeup();

		for(auto & pthread : _threads) {
			pthread->join();
		}
	}
    _threads.clear();
}

Threadpool::~Threadpool()
{
	if(!_isExit) {
		stop();
	}
}


void Threadpool::addTask(Task && task)
{
	_taskQue.push(std::move(task));
}

Task Threadpool::getTask()
{
	return _taskQue.pop();
}

//子线程要做的事情
void Threadpool::threadFunc()
{
	while(!_isExit)
	{
		Task task = getTask();
		if(task) {
			task();//执行任务的时间不确定
		}
	}
}
