#include "Acceptor.h"
#include "SocketUtil.h"

#include <iostream>
using std::cout;
using std::endl;


namespace wd
{

Acceptor::Acceptor(int listenfd, const InetAddress & addr)
: listenSock_(listenfd)
, addr_(addr)
{}

void Acceptor::ready()
{
	setReuseAddr(true);
	setReusePort(true);
	bind();
	listen();
}

int Acceptor::accept()
{
	int peerfd = ::accept(listenSock_.fd(), NULL, NULL);
	if(peerfd == -1)
	{
		perror("accept error");
	}
	return peerfd;
}

void Acceptor::setReuseAddr(bool flag)
{
	int on = (flag ? 1 : 0);
	if(::setsockopt(listenSock_.fd(), 
					SOL_SOCKET, 
					SO_REUSEADDR, 
					&on, 
					static_cast<socklen_t>(sizeof(on))) == -1)
	{
		perror("setsockopt reuseaddr error");
		::close(listenSock_.fd());
		exit(EXIT_FAILURE);
	}
}


void Acceptor::setReusePort(bool flag)
{
#ifdef SO_REUSEPORT
	int on = (flag ? 1 : 0);
	if(::setsockopt(listenSock_.fd(), 
				    SOL_SOCKET,
					SO_REUSEPORT,
					&on,
					static_cast<socklen_t>(sizeof(on))) == -1)
	{
		perror("setsockopt reuseport error");
		::close(listenSock_.fd());
		exit(EXIT_FAILURE);
	}
#else
	if(flag)
	{
		fprintf(stderr, "SO_REUSEPORT is not supported!\n");
	}
#endif

}

void Acceptor::bind()
{
	if(-1 == ::bind(listenSock_.fd(), 
					(const struct sockaddr*)addr_.getSockAddrPtr(), 
					sizeof(InetAddress)))
	{
		perror("bind error");
		::close(listenSock_.fd());
		exit(EXIT_FAILURE);
	}
}

void Acceptor::listen()
{
	if(-1 == ::listen(listenSock_.fd(), 10))
	{
		perror("listen error");
		::close(listenSock_.fd());
		exit(EXIT_FAILURE);
	}
}

}//end of namespace wd

