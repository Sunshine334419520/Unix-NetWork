/**
 * @Author: sunshine
 * @Date:   2017-11-10T17:02:36+08:00
 * @Email:  guang334419520@126.com
 * @Filename: tcpli02.c
 * @Last modified by:   sunshine
 * @Last modified time: 2017-11-23T15:30:20+08:00
 */



/*************************************************************************
    > File Name: tcpcli01.c
    > Author: ma6174
    > Mail: ma6174@163.com
    > Created Time: 2017年11月09日 星期四 16时18分28秒
 ************************************************************************/
#include "../unp.h"


void str_cli(FILE*, int);

int
main(int argc, char* argv[])
{
	if(argc != 2)
		err_quit("usage: tcpcli <IPaddress>");

	int i,sockfd[5];


	struct sockaddr_in servaddr;
	for(i = 0; i < 5; i++) {
	sockfd[i] = Socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	Connect(sockfd[i], (SA*)&servaddr, sizeof(servaddr));
	}
	str_cli(stdin, sockfd[0]);

	exit(0);
}

void
str_cli(FILE* fp, int sockfd)
{
	char sendline[MAXLINE],recvline[MAXLINE];

	while(Fgets(sendline,MAXLINE,fp) != NULL) {
		Writen(sockfd,sendline,strlen(sendline));

		if(Readline(sockfd, recvline, MAXLINE) == 0)
			err_quit("str_cli: server terminated prematurely");

		Fputs(recvline, stdout);
	}
}
