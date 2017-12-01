/*************************************************************************
    > File Name: udpserv.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: 2017年11月16日 星期四 16时20分54秒
 ************************************************************************/

#include "unp/unp.h"
#include "unp/apuerror.h"

void dg_echo(int, SA*, socklen_t);

int 
main(int argc, char* args[])
{
	int sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in servaddr,cliaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	Bind(sockfd, (SA*)&servaddr, sizeof(servaddr));

	dg_echo(sockfd, (SA*)&cliaddr, sizeof(cliaddr));
	return 0;
}

void 
dg_echo(int sockfd, SA* pcliaddr, socklen_t clilen)
{
	char msg[MAXLINE];
	socklen_t len;
	int n;

	for( ; ; ) {
		len = clilen;
		n = Recvfrom(sockfd, msg, MAXLINE, 0, pcliaddr, &clilen);

		Sendto(sockfd, msg, n, 0, pcliaddr, len);

	}
}
