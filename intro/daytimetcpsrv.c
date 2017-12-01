/*************************************************************************
    > File Name: daytimetcpsrv.c
    > Author: ma6174
    > Mail: ma6174@163.com
    > Created Time: 2017年11月02日 星期四 17时46分20秒
 ************************************************************************/
#include "../unp.h"
#include <time.h>

int
main(int argc, char* argv[])
{
	int listenfd, connfd;
	struct sockaddr_in servaddr;
	char buff[MAXLINE];
	time_t ticks;
/*
	if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			err_sys("socket error");
*/
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(13);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	/*
	if(bind(listenfd, (SA*)&servaddr, sizeof(servaddr)) == -1)
		err_sys("bind error");
	*/
	Bind(listenfd, (SA*)&servaddr, sizeof(servaddr));
/*	if(listen(listenfd, LISTENQ) < 0)
		err_sys("listen error");
*/
	Listen(listenfd, LISTENQ);

	for( ; ; ) {
		/*
		if((connfd = accept(listenfd, (SA*)NULL, NULL)) < 0)
			err_sys("accept error");
		*/
		connfd = Accept(listenfd, (SA*)NULL, NULL);
		ticks = time(NULL);
		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		printf("%s", buff);
		if(write(connfd, buff, strlen(buff)) < 0)
			err_sys("write error");
		close(connfd);
	}

}
