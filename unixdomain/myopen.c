/**
 * @Author: sunshine
 * @Date:   2017-12-06T17:07:44+08:00
 * @Email:  guang334419520@126.com
 * @Filename: myopen.c
 * @Last modified by:   sunshine
 * @Last modified time: 2017-12-06T18:47:57+08:00
 */


#include "unp/unp.h"


/**
 * 打开一个文件，通过派生的进程
 * @param  pathname 文件路径名
 * @param  mode     打开文件的方式
 * @return          放回一个描述符
 */
ssize_t read_fd(int, void*, size_t, int*);
int my_open(const char* pathname, int mode)
{
  int fd, sockfd[2], status;
  pid_t childpid;
  char c, argsockfd[10], argmode[10];
  fd = -1;
  //创建随后连接起来的2个套接字
  Socketpair(AF_LOCAL, SOCK_STREAM, 0, sockfd);

  if( (childpid = Fork()) == 0) { /* 子进程 */
    Close(sockfd[0]);         //关闭描述符0

    snprintf(argsockfd, sizeof(argsockfd), "%d", sockfd[1]);
    snprintf(argmode, sizeof(argmode), "%d", mode);

    execl("./openfile", "openfile", argsockfd,
          pathname, argmode, (char*)NULL);
    err_sys("execl error");
  }

  /* 关闭父进程描述符1 */
  Close(sockfd[1]);

  Waitpid(childpid, &status, 0);    //等待子进程关闭，然后接着运行

  if(WIFEXITED(status) == 0)        //检查子进程是否正常终止，不是则退出
    err_quit("child did not terminate");

  if( (status = WEXITSTATUS(status)) == 0) {  //检查子进程退出状态
    if (read_fd(sockfd[0], &c, 1, &fd) < 0)        //获取子进程描述符
      err_quit("read_fd error");
  }
  else {
    errno = status;
    fd = -1;
  }

  Close(sockfd[0]);
  return (fd);
}
