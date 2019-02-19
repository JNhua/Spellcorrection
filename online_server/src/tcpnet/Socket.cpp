#include "Socket.h"
#include "SocketUtil.h"
#include "InetAddress.h"

namespace wd
{

Socket::Socket(int sockfd)
: sockfd_(sockfd)
{}

Socket::Socket()
: sockfd_(createSocketFd())
{
}

Socket::~Socket()
{
	::close(sockfd_);
}

void Socket::nonblock()
{
	setNonblock(sockfd_);
}


void Socket::shutdownWrite()
{
	if(::shutdown(sockfd_, SHUT_WR) == -1)
	{
		perror("shudown write error!");
	}
}


InetAddress Socket::getLocalAddr(int sockfd)
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(sockaddr_in);
	if(::getsockname(sockfd, (struct sockaddr *)&addr, &len) == -1)
	{
		perror("getsockname error");
	}
	return InetAddress(addr);
}

InetAddress Socket::getPeerAddr(int sockfd)
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(sockaddr_in);
	if(::getpeername(sockfd, (struct sockaddr *)&addr, &len) == -1)
	{
		perror("getpeername error");
	}
	return InetAddress(addr);
}

}// end of namespace wd
