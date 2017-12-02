/**
 * @Author: sunshine
 * @Date:   2017-12-02T15:53:11+08:00
 * @Email:  guang334419520@126.com
 * @Filename: connect_timeo.c
 * @Last modified by:   sunshine
 * @Last modified time: 2017-12-02T16:04:39+08:00
 */

#include "unp/unp.h"

int connect_timeo(int sockfd, const SA* saptr, strlen_t salen, int nsec)
{
  Sigfunc* sigfunc;
  int n;

  sigfunc = Signal(SIGALARM, connect_alarm);
  if(alarm(nsec) != 0)
    err_msg("connect_timeo: alarm was already set");

  if( (n = connect(sockfd, saptr, salen)) < 0) {
    Close(sockfd);
    if (errno = EINTR) {
      errno = ETIMEOUT;
    }
  }

  alarm(0);
  Signal(SIGALRM,sigfunc);

  return n;

}

static void connect_alarm(int signo){
  return ;
}
