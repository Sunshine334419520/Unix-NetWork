/**
 * @Author: sunshine
 * @Date:   2017-11-25T15:30:30+08:00
 * @Email:  guang334419520@126.com
 * @Filename: tcp_connect.c
 * @Last modified by:   sunshine
 * @Last modified time: 2017-11-25T16:07:44+08:00
 */
#include "../../unp.h"

int
tcp_connect(const char* host, const char* serv)
{
  int sockfd, n;
  struct addrinfo hints, *res, *ressave;

  bzero(&hints, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if( (n = getaddrinfo(host, serv, &hints, &res)) != 0)
    err_quit("tcp_connect error for %s, %s: %s",
             host, serv, gai_strerror(n));
    ressave = res;

    do {
      sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
      if(sockfd < 0)
        continue;

      if(connect(sockfd, (SA*)&res->ai_addr, res->ai_addrlen) == 0)
        break;

      Close(sockfd);
    } while( (res = res->ai_next) != NULL);

    if(res == NULL) {
      err_sys("tcp_connect error %s, %s", host, serv);
    }

    freeaddrinfo(ressave);
    return sockfd;
}
