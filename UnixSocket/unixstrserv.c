/**
 * @Author: sunshine
 * @Date:   2017-12-06T16:29:20+08:00
 * @Email:  guang334419520@126.com
 * @Filename: unixstrserv.c
 * @Last modified by:   sunshine
 * @Last modified time: 2017-12-06T16:59:48+08:00
 */

#include "unp/unp.h"

void str_echo(int sockfd)
{
  char buff[MAXLINE];
  size_t n;

  while( (n = read(sockfd, buff, MAXLINE)) > 0)
    Write(sockfd, buff, n);
}

void sig_chld(int signo)
{
  int error = errno;
  pid_t pid;
  int stat;

  while( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
    printf("child %d terminated\n", pid);

  errno = error;
  return ;
}
int main(int argc, char const *argv[]) {
  int listenfd, connfd;
  pid_t childpid;
  socklen_t clilen;
  struct sockaddr_un cliaddr, servaddr;

  listenfd = Socket(AF_LOCAL, SOCK_STREAM, 0);

  unlink(UNIXSTR_PATH);

  bzero(&servaddr, sizeof(servaddr));

  servaddr.sun_family = AF_LOCAL;
  strcpy(servaddr.sun_path, UNIXSTR_PATH);

  Bind(listenfd, (SA*)&servaddr, sizeof(servaddr));

  Listen(listenfd, LISTENQ);

  Signal(SIGCHLD, sig_chld);

  for( ; ; ) {
    clilen = sizeof(cliaddr);
    if( (connfd = accept(listenfd, (SA*)&cliaddr, &clilen)) < 0) {
      if(errno == EINTR)
        continue;
      else
        err_sys("accept error");
    }

    if( (childpid = Fork()) == 0) {
      Close(listenfd);
      str_echo(connfd);
      exit(0);
    }
    Close(connfd);
  }

  return 0;
}
