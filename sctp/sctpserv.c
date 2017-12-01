/*************************************************************************
    > File Name: sctpserv.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: 2017年11月19日 星期日 16时28分53秒
 ************************************************************************/

#include "unp/unp.h"
#include "unp/apuerror.h"

int 
main(int argc, char* argv[])
{
	int stream_increment = 1;
	int msg_flags;
	struct sctp_sndrcvinfo sri;
	size_t n, rd_sz;
	char buf[MAXLINE];
	socklen_t len;

	if(argc == 2)
		stream_increment = atoi(argv[1]);
	
	int sock_fd = Socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);

	struct sockaddr_in servaddr, cliaddr;
	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	Bind(sock_fd, (SA*)&servaddr, sizeof(servaddr));

	struct sctp_event_subscribe evnts;
	bzero(&evnts,sizeof(evnts));
	
	evnts.sctp_data_io_event = 1;
	Setsockopt(sock_fd, IPPROTO_SCTP, SCTP_EVENTS, &evnts, sizeof(evnts));

	Listen(sock_fd, LISTENQ);
	for( ; ;  ) {
		len = sizeof(struct sockaddr_in);
		rd_sz = Sctp_recvmsg(sock_fd,buf,MAXLINE,
							 (SA*)&cliaddr, &len, &sri, &msg_flags);
		
		if(stream_increment) {
			sri.sinfo_stream++;
		}

		Sctp_sendmsg(sock_fd, buf, rd_sz,
					 (SA*)&cliaddr, len,
					 sri.sinfo_ppid,
					 sri.sinfo_flags, sri.sinfo_stream, 0, 0);
	}


	return 0;
}
