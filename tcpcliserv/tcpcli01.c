/**
 * @Author: sunshine
 * @Date:   2017-11-18T17:44:13+08:00
 * @Email:  guang334419520@126.com
 * @Filename: tcpcli01.c
 * @Last modified by:   sunshine
 * @Last modified time: 2017-12-03T15:33:18+08:00
 */



#include "../unp.h"
void str_cli(FILE*, int);

int
main(int argc, char* argv[])
{
	if(argc != 2)
		err_quit("usage: tcpcli <IPaddress>");

	int sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	Connect(sockfd, (SA*)&servaddr, sizeof(servaddr));

	str_cli(stdin, sockfd);

	exit(0);
}
/*
void str_cli(int socket) {
	char line[MAXLINE];
	FILE* fpin, *fpout;
	fpin = Fdopen(socket, "r");
	fpout = Fdopen(socket, "w");

	while (Fgets(line, MAXLINE, fpin) != NULL) {
		Fputs(line, fpout);
	}
}
*/

/*
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
*/


/**
 * Select IO/复用
 *
 * @param fp 文件描述符
 * @param sockfd 套接字描述符
 */
void
str_cli(FILE* fp, int sockfd)
{
	int maxfdp1, stdineof;
	fd_set rset;
	char buf[MAXLINE];
	int n;

	stdineof = 0;
	FD_ZERO(&rset);

	for( ; ; ) {
		if(stdineof == 0)
			FD_SET(fileno(fp), &rset);
		FD_SET(sockfd, &rset);

		maxfdp1 = max(fileno(fp), sockfd) + 1;
		Select(maxfdp1, &rset, NULL, NULL, NULL);

		if(FD_ISSET(sockfd, &rset)) {
			if( (n = read(sockfd, buf, MAXLINE)) < 0)
				err_sys("read error");
			else if(n == 0) {
				if(stdineof == 1)
					return;
				else
					err_quit("str_cli: server terminated prematurely");
			}

			Writen(fileno(stdout), buf, n);
		}

		if(FD_ISSET(fileno(fp), &rset)) {
			if( (n = read(fileno(fp), buf, MAXLINE)) < 0)
				err_sys("read error");
			else if(n == 0) {
				stdineof = 1;
				Shutdown(sockfd, SHUT_WR);
				FD_CLR(fileno(fp), &rset);
				continue;
			}
			Writen(sockfd, buf, n);
		}

	}
}
