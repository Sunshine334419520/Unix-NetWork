/*************************************************************************
    > File Name: wrapsock.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: 2017年11月04日 星期六 20时18分33秒
 ************************************************************************/
#include "unp/unp.h"
#include "unp/apuerror.h"
#include "unp/myreadwrite.h"
int
Socket(int family, int type, int protocol)
{
	int n;

	if( (n = socket(family, type, protocol)) < 0)
		err_sys("socket error");
	return(n);
}


void 
Connect(int sockfd, const struct sockaddr* servaddr, socklen_t addrlen)
{
	if(connect(sockfd, servaddr, addrlen) < 0) 
		err_sys("Connect error:");
	return ;
}

void
Bind(int sockfd, const struct sockaddr* myaddr, socklen_t addrlen)
{
	if(bind(sockfd,myaddr,addrlen) < 0)
		err_sys("Bind error:");

	return ;
}

void
Listen(int sockfd, int backlog)
{
	char* ptr;

	if( (ptr = getenv("LISTENQ")) != NULL)
		backlog = atoi(ptr);

	if(listen(sockfd, backlog) < -1)
		err_sys("Listen error");

	return ;
}

int 
Accept(int sockfd, struct sockaddr* cliaddr, socklen_t* addrlen)
{
	int n;

	if( (n = accept(sockfd, cliaddr, addrlen)) < 0)
		err_sys("Accept error");

	return n;
}

pid_t 
Fork(void)
{
	pid_t pid;
	if( (pid = fork()) < 0)
		err_sys("fork error");
	return pid;
}

void
Inet_pton(int family, const char* strptr, void* addrptr)
{
	int n;
	if( (n = inet_pton(family, strptr, addrptr)) < 0)
		err_sys("Inet_pton error");
	else if(n == 0) {
		err_quit("%s no addr", strptr);
	}

}

const char* 
Inet_ntop(int family, const void* addrptr, char* strptr, size_t len)
{
	const char* ptr;
	if( (ptr = inet_ntop(family, addrptr, strptr, len)) == NULL)
		err_sys("inet_ntop error");
	return ptr;
}



/**
 * @brief 
 *
 *MyReadn MyWritn Myreadnline
 */
ssize_t 
Readn(int fd, void* buff, size_t nbytes) 
{
	ssize_t n;
	if( (n = readn(fd, buff, nbytes)) < 0)
		err_sys("Readn error");

	return n;
}

void 
Writen(int fd, void* buff, size_t nbytes)
{
	ssize_t n;
	if( (n = writen(fd, buff, nbytes)) < 0) 
		err_sys("Writen error");

	return ;
}

ssize_t
Readline(int fd, void* buff, size_t nbytes)
{
	ssize_t n;
	if( (n = readline(fd, buff, nbytes)) < 0)
		err_sys("Writen error");

	return n;
}


