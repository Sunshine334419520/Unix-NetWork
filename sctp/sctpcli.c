/*************************************************************************
    > File Name: sctpcli.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: 2017年11月22日 星期三 16时10分11秒
 ************************************************************************/

#include "unp/unp.h"
#include "unp/apuerror.h"

void sctpstr_cli(FILE*, int, struct sockaddr*, socklen_t);

int
main(int argc, char* argv[])
{
	int sock_fd;
	struct sockaddr_in servaddr;
	struct sctp_event_subscribe evnts;
	int echo_to_all = 0;

	if(argc < 2) {
		err_quit("Missing host argument - use '%s host [echo]'\n",argv[0]);
	}
	if(argc > 2) {
		echo_to_all = 1;
	}

	sock_fd = Socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);

	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	bzero(&evnts, sizeof(evnts));
	evnts.sctp_data_io_event = 1;
	Setsockopt(sock_fd, IPPROTO_SCTP, SCTP_EVENTS, &evnts, sizeof(evnts));

	if(echo_to_all == 0) 
		sctpstr_cli(stdin, sock_fd, (SA*)&servaddr, sizeof(servaddr));
	else 
		//sctpstr_cli_echoall(stdin,sock_fd, (SA*)&servaddr, sizeof(servaddr));

	Close(sock_fd);
	return 0;
}


void
sctpstr_cli(FILE* fp, int sock_fd, struct sockaddr* to, socklen_t tolen)
{
	struct sockaddr_in peeraddr;
	struct sctp_sndrcvinfo sri;
	char sendline[MAXLINE],recvline[MAXLINE];
	socklen_t len;
	int out_sz, rd_sz;
	int msg_flags;

	bzero(&sri, sizeof(sri));
	while(fgets(sendline, MAXLINE, fp) != NULL) {
		if(sendline[0] != '[') {
			printf("Error, line must be of the form '[streamnum]text'\n");
			continue;
		}

		sri.sinfo_stream = strtol(&sendline[1], NULL, 0);
		out_sz = strlen(sendline);
		Sctp_sendmsg(sock_fd, sendline, out_sz,
					 to, tolen, 0, 0, sri.sinfo_stream, 0, 0);

		len = sizeof(peeraddr);
		rd_sz = Sctp_recvmsg(sock_fd, recvline, sizeof(recvline),
				(SA*)&peeraddr, &len, &sri, &msg_flags);

		printf("From str:%d seq: %d (assoc:0x%x):",
				sri.sinfo_stream, sri.sinfo_ssn, (u_int)sri.sinfo_assoc_id);
		printf("%.*s",rd_sz, recvline);
	}
}
