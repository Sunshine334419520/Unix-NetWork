/**
 * @Author: sunshine
 * @Date:   2017-11-18T17:45:16+08:00
 * @Email:  guang334419520@126.com
 * @Filename: tcpserv01.c
 * @Last modified by:   sunshine
 * @Last modified time: 2017-12-03T15:38:44+08:00
 */

#include "unp/unp.h"
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

/*
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
*/
void str_echo(int socket) {
	char line[MAXLINE];
	FILE *fpin, *fpout;
	fpin = Fdopen(socket, "r");
	fpout = Fdopen(socket, "w");

	while (Fgets(line, MAXLINE, fpin) != NULL) {
		Fputs(line, fpout);
	}
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
