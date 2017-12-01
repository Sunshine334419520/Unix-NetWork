/*************************************************************************
    > File Name: tcpudpsele.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: 2017年11月18日 星期六 16时20分58秒
 ************************************************************************/
#include "unp/unp.h"
#include "unp/apuerror.h"


void sig_chld(int);
void str_echo(int);

int 
main(int argc, char* argv[])
{
	int nready,connfd, len;
	pid_t childpid;
	char mesg[MAXLINE];
	ssize_t n;

	int listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in cliaddr,servaddr;
	bzero(&servaddr,sizeof(servaddr));
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	const int on = 1;
	Setsockopt(listenfd,SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	Bind(listenfd, (SA*)&servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);

	
	
	int udpfd = Socket(AF_INET, SOCK_DGRAM, 0);
	
	bzero(&servaddr, sizeof(servaddr));
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	Bind(udpfd, (SA*)&servaddr, sizeof(servaddr));
	
	signal(SIGCHLD, sig_chld);
	
	fd_set rest;
	FD_ZERO(&rest);
	int maxfdp1 = max(listenfd, udpfd) + 1;
	
	for( ; ; ) {
		
		
		FD_SET(udpfd, &rest);
		FD_SET(listenfd, &rest);
		if( (nready = select(maxfdp1, &rest, NULL, NULL, NULL)) < 0) {
			if(errno == EINTR)
				continue;
			else
				err_sys("select error");
		}

		if(FD_ISSET(listenfd, &rest)) {
			len = sizeof(cliaddr);
			connfd = Accept(listenfd, (SA*)&cliaddr, &len);

			if( (childpid = Fork()) == 0) {
				Close(listenfd);
				str_echo(connfd);
				exit(0);
			}
			Close(connfd);
		}


		if(FD_ISSET(udpfd, &rest)) {
			len = sizeof(cliaddr);
			n = Recvfrom(udpfd, mesg, MAXLINE, 0, (SA*)&cliaddr, &len);

			Sendto(udpfd, mesg, n, 0, (SA*)&cliaddr, len);
		}
	}

	return 0;
}


void 
str_echo(int sockfd)
{
	ssize_t n;
	char buff[MAXLINE];
again:
	while( (n = read(sockfd, buff, MAXLINE)) > 0)
		Writen(sockfd, buff, n);

	if(n < 0 && errno == EINTR)
		goto again;
	else if(n < 0)
		err_sys("str_echo: read error");
}

/*
void str_echo(int sockfd)
{
	long arg1, arg2;
	ssize_t n;
	char line[MAXLINE];

	for( ; ; ) {
		if( (n = Readline(sockfd, line, MAXLINE)) == 0)
			return;
		if(sscanf(line, "%ld%ld", &arg1, &arg2) == 2)
			snprintf(line, sizeof(line),"%ld\n", arg1 + arg2);
		else 
			snprintf(line,sizeof(line), "input error\n");
		n = strlen(line);
		Writen(sockfd, line, n);
	}
}
*/

void
sig_chld(int sig)
{
	pid_t pid;
	int stat;

	while( (pid = waitpid(-1, &stat, WNOHANG)) > 0) 
		printf("child %d terminated\n", pid);
	return ;
}
