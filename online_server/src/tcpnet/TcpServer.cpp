#include "TcpServer.h"
#include "InetAddress.h"
#include "SocketUtil.h"
#include <iostream>
using std::cout;
using std::endl;


namespace wd
{

TcpServer::TcpServer(const string & ip, unsigned short port)
: acceptor_(createSocketFd(), InetAddress(ip.c_str(), port))
, poller_(acceptor_)
{
}

void TcpServer::start()
{
	acceptor_.ready();
	poller_.setConnectionCallback(connectionCallback_);
	poller_.setMessageCallback(messageCallback_);
	poller_.setCloseCallback(closeCallback_);
	poller_.loop();
}

void TcpServer::stop()
{
	poller_.unloop();
}
	
void TcpServer::setConnectionCallback(TcpServerCallback cb)
{	connectionCallback_ = cb;	}
void TcpServer::setMessageCallback(TcpServerCallback cb)
{	messageCallback_ = cb;	}
void TcpServer::setCloseCallback(TcpServerCallback cb)
{	closeCallback_ = cb;	}

}//end of namespace wd
