/*************************************************************************
    > File Name: tcpserv01.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: 2017年11月09日 星期四 15时51分45秒
 ************************************************************************/
#include "unp/unp.h"
#include "unp/apuerror.h"
void str_echo(int );
void sig_chld(int);
int 
main(int argc, char* argv[])
{
	int listenfd,connfd;
	pid_t childpid;
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in cliaddr,servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	Bind(listenfd, (SA*)&servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);
	
	signal(SIGCHLD, sig_chld);
	
	socklen_t clilen;
	for(; ;) {
		bzero(&cliaddr, sizeof(cliaddr));
		if( (connfd = accept(listenfd, (SA*)&cliaddr, &clilen)) < 0) {
			if(errno == EINTR)
				continue;
			else
				err_sys("accept error");
		}

		//有连接进入，新建一个进程进行通信
		if( (childpid = Fork()) == 0) {
			close(listenfd);
			str_echo(connfd);
			exit(0);
		}
		close(connfd);
	}
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
