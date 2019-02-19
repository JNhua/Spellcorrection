#include "EpollPoller.h"
#include "SocketUtil.h"
#include "Acceptor.h"
#include <assert.h>
#include <iostream>
using std::cout;
using std::endl;


namespace wd
{

EpollPoller::EpollPoller(Acceptor & acceptor)
: acceptor_(acceptor)
, epollfd_(createEpollFd())
, eventfd_(createEventFd())
, listenfd_(acceptor_.fd())
, isLooping_(false)
, eventsList_(1024)
{
	addEpollFdRead(epollfd_, listenfd_);
	addEpollFdRead(epollfd_, eventfd_);
}

EpollPoller::~EpollPoller()
{
	::close(epollfd_);
}

void EpollPoller::loop()
{
	isLooping_ = true;
	while(isLooping_)
	{
		waitEpollfd(); }
}

void EpollPoller::unloop()
{
	if(isLooping_)
		isLooping_ = false;
}


void EpollPoller::setConnectionCallback(EpollCallback cb)
{
	onConnectionCb_ = cb;
}

void EpollPoller::setMessageCallback(EpollCallback cb)
{
	onMessageCb_ = cb;
}

void EpollPoller::setCloseCallback(EpollCallback cb)
{
	onCloseCb_ = cb;
}

void EpollPoller::waitEpollfd()
{
	int nready;
	do
	{
		nready = ::epoll_wait(epollfd_, 
							  &(*eventsList_.begin()),
							  eventsList_.size(),
							  5000);
	}while(nready == -1 && errno == EINTR);

	if(nready == -1)
	{
        perror("epoll_wait error\n");
		exit(EXIT_FAILURE);
	}
	else if(nready == 0)
	{
		printf("epoll_wait timeout\n");	
	}
	else
	{
		//做一个扩容的操作
		if(nready == static_cast<int>(eventsList_.size()))
		{
			eventsList_.resize(eventsList_.size() * 2);
		}
		
		//遍历每一个激活的文件描述符
		for(int idx = 0; idx != nready; ++idx)
		{
			if(eventsList_[idx].data.fd == listenfd_)
			{
				if(eventsList_[idx].events & EPOLLIN)
				{
					handleConnection();
				}
			}
			else if(eventsList_[idx].data.fd == eventfd_)
			{
				handleRead();
				doPendingFunctors();
			}
			else 
			{
				if(eventsList_[idx].events & EPOLLIN)
				{
					handleMessage(eventsList_[idx].data.fd);
				}
			}
		}//end for
	}//end else
}

void EpollPoller::handleConnection()
{
	int peerfd = acceptor_.accept(); 
	addEpollFdRead(epollfd_, peerfd);

	TcpConnectionPtr conn(new TcpConnection(peerfd, this));
	conn->setConnectionCallback(onConnectionCb_);
	conn->setMessageCallback(onMessageCb_);
	conn->setCloseCallback(onCloseCb_);

	std::pair<ConnectionMap::iterator, bool> ret;
	ret = connMap_.insert(std::make_pair(peerfd, conn));
	assert(ret.second == true);
	(void)ret;
	conn->handleConnectionCallback();
}

void EpollPoller::handleMessage(int peerfd)
{
	bool isClosed = isConnectionClosed(peerfd);
	ConnectionMap::iterator it = connMap_.find(peerfd);
	assert(it != connMap_.end());

	if(isClosed)
	{
		it->second->handleCloseCallback();
		delEpollReadFd(epollfd_, peerfd);
		connMap_.erase(it);
	}
	else
	{
		it->second->handleMessageCallback();
	}
}


//在计算线程中执行
void EpollPoller::runInLoop(const Functor && cb)
{
	{
		MutexLockGuard mlg(_mutex);
		_pendingFunctors.push_back(std::move(cb));
	}
	wakeup();
}

void EpollPoller::doPendingFunctors()
{
	std::vector<Functor> tmp;

	{
		MutexLockGuard mlg(_mutex);
		tmp.swap(_pendingFunctors);
	}
	for(auto & functor : tmp)
	{
		functor();
	}
}

void EpollPoller::handleRead()
{
	uint64_t howmany;
	int ret = ::read(eventfd_, &howmany, sizeof(howmany));
	if(ret != sizeof(howmany))
	{
		perror("read error");
	}
}

void EpollPoller::wakeup()
{
	uint64_t one = 1;
	int ret = ::write(eventfd_, &one, sizeof(one));
	if(ret != sizeof(one))
	{
		perror("write error");
	}
}

}// end of namespace wd
