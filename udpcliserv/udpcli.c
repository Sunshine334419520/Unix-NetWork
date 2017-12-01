/*************************************************************************
    > File Name: udpcli.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: 2017年11月16日 星期四 16时51分22秒
 ************************************************************************/
#include "unp/unp.h"
#include "unp/apuerror.h"

void dg_cli(FILE*, int, const SA*, socklen_t);

int
main(int argc, char* args[])
{
	if(argc != 2)
		err_quit("Usage:udpcli <IPaddress>");
	int sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
	
	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	Inet_pton(AF_INET, args[1], &servaddr.sin_addr);

	dg_cli(stdin, sockfd, (SA*)&servaddr, sizeof(servaddr));
	return 0;
}

/*
void 
dg_cli(FILE* fp, int sockfd, const SA* pservaddr, socklen_t servlen)
{
	char sendline[MAXLINE], recvline[MAXLINE + 1];
	socklen_t len;
	int n;
	struct sockaddr* preply_addr = (struct sockaddr*)malloc(servlen);
	while(Fgets(sendline, MAXLINE, fp) != NULL) {

		Sendto(sockfd, sendline, MAXLINE, 0, pservaddr, servlen);
		len = servlen;

		n = Recvfrom(sockfd, recvline, MAXLINE, 0, preply_addr, &len);

		if(len != servlen || memcmp(preply_addr, pservaddr, len) != 0) {
			printf("reply from %s (ignored)\n",Sock_ntop(preply_addr, len));
			continue;
		}

		recvline[n] = 0;
		Fputs(recvline, stdout);
	}
}
*/

void
dg_cli(FILE* fp, int sockfd, const SA* pservaddr, socklen_t servlen)
{
	char sendline[MAXLINE], recvline[MAXLINE + 1];
	int n;

	Connect(sockfd, (SA*)pservaddr, servlen);

	while(Fgets(sendline, MAXLINE, fp) != NULL) {
		if(write(sockfd, sendline, MAXLINE) < 0)
			err_sys("write error");

		if( (n = read(sockfd, recvline, MAXLINE)) < 0) 
			err_sys("read error");

		recvline[n] = 0;
		Fputs(recvline, stdout);
	}
}


