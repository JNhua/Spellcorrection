#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include "json/json.h"
using std::string;
using std::endl;
using std::cout;

#define ERR_EXIT(m) \
	do { \
		perror(m);\
		exit(EXIT_FAILURE);\
	}while(0)

void do_service(int sockfd);
size_t readn(int sockfd,char *buf,size_t count)
{
	size_t nleft = count;
	char * pbuf = buf;
	while(nleft > 0)
	{
		int nread = read(sockfd,pbuf,nleft);
		if( nread == -1)
		{
			if(errno == EINTR)
				continue;
			return EXIT_FAILURE;
		}else if(nread == 0 )
			break;
		pbuf += nread;
		nleft -= nread;
	}
	return (count - nleft);
}
size_t writen(int sockfd,const char *buf,size_t count)
{
	size_t nleft = count;
	const char * pbuf = buf;
	while(nleft > 0)
	{
		int nwrite = write(sockfd,pbuf,nleft);
		if(nwrite == -1)
		{
			if(errno == EINTR)
				continue;
			return EXIT_FAILURE;
		}
		nleft -= nwrite;
		pbuf += nwrite;
	}
	return (count -nleft);
}
int main(int argc, const char *argv[])
{
	if(argc !=3)
	{
		printf("./client IP port\n");
		return -1;
	}
	int peerfd = socket(PF_INET, SOCK_STREAM, 0);
	if(peerfd == -1)
		ERR_EXIT("socket");

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof addr);
	addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]); //localhost
    addr.sin_port = htons(atoi(argv[2]));
	socklen_t len = sizeof addr;
	if(connect(peerfd, (struct sockaddr*)&addr, len) == -1)
		ERR_EXIT("Connect");

	char buf[1024];
	int length;
	memset(buf, 0, sizeof(buf));
	readn(peerfd,(char*)&length,sizeof(int));
	readn(peerfd, buf, length);
	printf("%s\n", buf);

	do_service(peerfd);


	return 0;
}

void jsonParse(const string& json_str)
{
	Json::Reader reader;
	Json::Value items;
	reader.parse(json_str,items);
	int item_num  = items.size();
	for(int i = 0;i< item_num ; ++i)
	{
		Json::Value item = items[i];
		string split_str = item.toStyledString();
		cout << split_str;
	}
}


void do_service(int sockfd)
{
	char recvbuf[1024] = {0};
	char sendbuf[1024] = {0};
	int epfd = epoll_create1(0);
	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = 0;
	epoll_ctl(epfd,EPOLL_CTL_ADD,0,&event);
	event.data.fd = sockfd;
	epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&event);
	struct epoll_event envs[2];
	int nready;
	while(1)
	{
		nready = epoll_wait(epfd,envs,2,5000);
		if(nready == -1)
		{
			perror("epoll_wait error");
			exit(EXIT_FAILURE);
		}else if(nready == 0)
			printf("epoll_wait timeout\n");
		else{
			for(int i=0;i<nready;++i)
			{
				if(envs[i].data.fd == 0)
				{
					bzero(sendbuf,sizeof(sendbuf));
					read(0,sendbuf,sizeof(sendbuf)-1);
					writen(sockfd,sendbuf,strlen(sendbuf));
				}else if(envs[i].data.fd == sockfd)
				{
					int len;
					bzero(recvbuf,sizeof(recvbuf));
					readn(sockfd,(char*)&len,sizeof(int));
					readn(sockfd,recvbuf,len);
					jsonParse(string(recvbuf));
				}
			}
		}
	}
}




