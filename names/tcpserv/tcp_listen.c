/**
 * @Author: sunshine
 * @Date:   2017-11-25T16:14:09+08:00
 * @Email:  guang334419520@126.com
 * @Filename: tcp_listen.c
 * @Last modified by:   sunshine
 * @Last modified time: 2017-11-25T16:53:29+08:00
 */
#include "../../unp.h"

int
tcp_listen(const char* hostname, const char* service, socklen_t* addrlenp)
{
    int listenfd, n;

    struct addrinfo hints, *res, *ressave;

    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM;

    if( (n = getaddrinfo(hostname, service, &hints, &res)) != 0) {
      err_quit("tcp_listen error for %s, %s: %s",
               hostname, service, gai_strerror(n));
    }

    int on = 1;
    ressave = res;

    do {
      listenfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
      if(listenfd < 0)
        continue;

      Setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
      if(bind(listenfd, (SA*)&res->ai_addr, res->ai_addrlen) == 0)
        break;
    } while((res = res->ai_next) != NULL);

    if(res == NULL) {
      err_sys("tcp_listen error for %s, %s", hostname, service);
    }

    Listen(listenfd, LISTENQ);

    freeaddrinfo(ressave);

    return (listenfd);
}
