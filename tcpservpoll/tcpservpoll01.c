/*************************************************************************
    > File Name: tcpserv01.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: 2017年11月09日 星期四 15时51分45秒
 ************************************************************************/
#include "unp/unp.h"
#include "unp/apuerror.h"
#include <limits.h>
#define OPEN_MAX 256
int 
main(int argc, char* argv[])
{
	int listenfd,connfd, sockfd;
	int nready;
	char buf[MAXLINE];
	ssize_t n;
	struct pollfd client[OPEN_MAX];
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in cliaddr,servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	Bind(listenfd, (SA*)&servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);
	
	
	client[0].fd= listenfd;
	client[0].events = POLLRDNORM;
	int i;
	for(i = 1; i < OPEN_MAX; i++) {
		client[i].fd = -1;
	}

	int maxi = 0;


	socklen_t clilen;
	for(; ;) {
		bzero(&cliaddr, sizeof(cliaddr));
		nready = Poll(client, maxi + 1, INFTIM);
		
		if(client[0].revents & POLLRDNORM) {
			connfd = Accept(listenfd, (SA*)&cliaddr, &clilen);
			
			for(i = 1; i < OPEN_MAX; i++) {
				if(client[i].fd < 0) {
					client[i].fd = connfd;
					client[i].events = POLLRDNORM;
					break;
				}
			}
			if(i == OPEN_MAX)
				err_quit("too many clients");

			if(i > maxi)
				maxi = i;
			if(--nready <= 0)
				continue;
		}

		for(i = 1; i <= maxi; i++) {
			if( (sockfd = client[i].fd) < 0)
				continue;
			
			if(client[i].revents & (POLLRDNORM | POLLERR)) {
				if( (n = read(sockfd, buf, MAXLINE)) <= 0) {
					if(errno == ECONNRESET || n == 0) {
						Close(sockfd);
						client[i].fd = -1;
					} else
						err_sys("read error");
				} else 
					Writen(sockfd, buf, n);

				if(--nready <= 0)
					break;
		
			}
		}

	}
}


