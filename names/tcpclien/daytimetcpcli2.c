/**
 * @Author: sunshine
 * @Date:   2017-11-25T15:44:58+08:00
 * @Email:  guang334419520@126.com
 * @Filename: daytimetcpcli2.c
 * @Last modified by:   sunshine
 * @Last modified time: 2017-11-25T16:04:25+08:00
 */
#include "../../unp.h"

int main(int argc, char const *argv[]) {
  int sockfd, n;

  if(argc != 3) {
    err_quit("usage: daytimetcpcli2 <hostname/IPaddress> <service/port#>");
  }

  sockfd = tcp_connect(argv[1], argv[2]);

  struct sockaddr_storage ss;
  socklen_t len = sizeof(ss);
  Getpeername(sockfd, (SA*)&ss, &len);
  printf("connected to %s\n",Sock_ntop( (SA*)&ss, len));

  char recvline[MAXLINE];

  while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
    recvline[n] = 0;
    Fputs(recvline, stdout);
  }
  return 0;
}
