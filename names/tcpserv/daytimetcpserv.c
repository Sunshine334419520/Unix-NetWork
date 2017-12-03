/**
 * @Author: sunshine
 * @Date:   2017-11-25T16:28:54+08:00
 * @Email:  guang334419520@126.com
 * @Filename: daytimetcpserv.c
 * @Last modified by:   sunshine
 * @Last modified time: 2017-11-25T16:51:54+08:00
 */

#include "../../unp.h"

int main(int argc, char const *argv[]) {
  int listenfd, connfd;
  socklen_t addrlen;

  if(argc == 2) {
    listenfd = tcp_listen(NULL, argv[1], &addrlen);
  }
  else if(argc == 3) {
    listenfd = tcp_listen(argv[1], argv[2], &addrlen);
  }
  else {
    err_quit("usage: daytimetcpserv [ <host> ] <service or prot>");
  }

  struct sockaddr_storage cliaddr;
  socklen_t len;
  time_t ticks;
  char buff[MAXLINE];
  while (TRUE) {
    len = sizeof(cliaddr);
    connfd = Accept(listenfd, (SA*)&cliaddr, &len);
    printf("connection from %s\n", Sock_ntop((SA*)&cliaddr, len));
    ticks = time(NULL);

    snprintf(buff, sizeof(buff), "%s.24s\r\n", ctime(&ticks));
    Write(connfd, buff, strlen(buff));

    Close(connfd);
  }
  return 0;
}
