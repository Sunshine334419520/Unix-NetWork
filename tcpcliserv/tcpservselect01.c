/**
 * @Author: sunshine
 * @Date:   2017-11-12T16:20:34+08:00
 * @Last modified by:   sunshine
 * File name: tcpserv01.c
 * @Last modified time: 2017-11-23T15:32:08+08:00
 */



/*************************************************************************
    > File Name: tcpserv01.c
    > Author: ma6174
    > Mail: ma6174@163.com
    > Created Time: 2017年11月09日 星期四 15时51分45秒
 ************************************************************************/
#include "../unp.h"


int
main(int argc, char* argv[])
{
	int listenfd,connfd, sockfd;
	int nready, client[FD_SETSIZE];
	char buf[MAXLINE];
	ssize_t n;
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in cliaddr,servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	Bind(listenfd, (SA*)&servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);

	int maxfd = listenfd;
	int maxi = -1;
	int i;
	for(i = 0; i < FD_SETSIZE; i++)
		client[i] = -1;

	fd_set allset,rest;
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);

	socklen_t clilen;
	for(; ;) {
		rest = allset;
		bzero(&cliaddr, sizeof(cliaddr));
		nready = Select(maxfd + 1, &rest, NULL, NULL, NULL);
		if(FD_ISSET(listenfd, &rest)) {
			connfd = Accept(listenfd, (SA*)&cliaddr, &clilen);

			for(i = 0; i < FD_SETSIZE; i++) {
				if(client[i] < 0) {
					client[i] = connfd;
					break;
				}
			}

			if(FD_SETSIZE == i)
				err_quit("too many clients");

			FD_SET(connfd, &allset);
			if(connfd > maxfd)
				maxfd = connfd;

			if(i > maxi)
				maxi = i;

			if(--nready <= 0)
				continue;
		}

		for(i = 0; i <= maxi; i++) {
			if( (sockfd = client[i]) < 0)
				continue;
			if(FD_ISSET(sockfd, &rest)) {
				if( (n = read(sockfd, buf, MAXLINE)) < 0)
					err_sys("read error");
				else if(n == 0) {
					Close(sockfd);
					client[i] = -1;
					FD_CLR(sockfd, &allset);
				} else
					Writen(sockfd, buf, n);

				if(--nready <= 0)
					break;
			}
		}

	}
}
