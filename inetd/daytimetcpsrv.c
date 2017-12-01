/**
 * @Author: sunshine
 * @Date:   2017-12-01T16:23:23+08:00
 * @Email:  guang334419520@126.com
 * @Filename: daytimetcpsrv.c
 * @Last modified by:   sunshine
 * @Last modified time: 2017-12-01T16:34:53+08:00
 */

#include "unp/unp.h"
#include <time.h>

int main(int argc, char const *argv[]) {
  int listenfd, connfd;
  socklen_t addrlen, len;
  struct sockaddr* cliaddr;
  char buff[MAXLINE];
  time_t ticks;

  if(argc < 2 || argc > 3)
    err_quit("usage: daytimetcpsrv2 [<host>] <service or port>");

  daemon_init(argv[0], 0);

  if(argc == 2)
    listenfd = Tcp_listen(NULL, argv[1], &addrlen);
  else
    listenfd = Tcp_listen(argv[1], argv[2], &addrlen);

  cliaddr = Malloc(addrlen);

  for( ; ; ) {
    len = addrlen;
    connfd = Accept(listenfd, cliaddr, &len);
    err_msg("connection from %s", Sock_ntop(cliaddr, len));

    ticks = time(NULL);
    snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
    Write(connfd, buff, strlen(buff));

    Close(connfd);
  }
  return 0;
}
