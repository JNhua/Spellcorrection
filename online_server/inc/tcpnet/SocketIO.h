#ifndef _WD_SOCKETIO_H
#define _WD_SOCKETIO_H
#include <stdio.h>

namespace wd
{
class SocketIO
{
public:
	SocketIO(int sockfd);

	size_t readn(char * buf, size_t count);
	size_t writen(const char * buf, size_t count);
	size_t readline(char * buf, size_t max_len);

private:
	size_t recv_peek(char * buf, size_t count);

private:
	int sockfd_;
};


}// end of namespace wd


#endif

