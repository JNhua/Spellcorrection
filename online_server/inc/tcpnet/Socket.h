#ifndef __WD_SOCKET_H
#define __WD_SOCKET_H

#include "Noncopyable.h"

namespace wd
{
class InetAddress;
class Socket : Noncopyable
{
public:
	Socket(int sockfd);
	Socket();
	~Socket();

	void shutdownWrite();
	int fd()const{	return sockfd_;	}

	void nonblock();

	static InetAddress getLocalAddr(int sockfd);
	static InetAddress getPeerAddr(int sockfd);
private:
	int sockfd_;
};
}// end of namespace wd

#endif
