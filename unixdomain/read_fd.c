/**
 * @Author: sunshine
 * @Date:   2017-12-06T17:31:47+08:00
 * @Email:  guang334419520@126.com
 * @Filename: read_fd.c
 * @Last modified by:   sunshine
 * @Last modified time: 2017-12-06T18:44:41+08:00
 */

#include "unp/unp.h"

/**
 * 从子进程获取描述符
 * @param  fd     用来通信的描述符
 * @param  ptr    接收缓冲区
 * @param  nbytes 缓冲区大小
 * @param  recvfd 存放放回的描述符
 * @return        返回读取内容大小
 */
ssize_t read_fd(int fd, void* ptr, size_t nbytes, int* recvfd)
{
  /* msghdr structure */
  struct msghdr msg;
  struct iovec iov[1];
  ssize_t n;
/*如果支持msg_control 就使用msg_control else 使用msg_accrights */
#ifdef HAVE_MSGHDR_MSG_CONTROL
  /* control_un 联合体用来保证control的对齐 */
  union {
    struct cmsghdr cm;
    char control[CMSG_SPACE(sizeof(int))];
  } control_un;

  struct cmsghdr* cmptr;

  msg.msg_control = control_un.control;
  msg.msg_controllen = sizeof(control_un.control);
#else
  int newfd;

  msg.msg_accrights = (caddr_t)&newfd;
  msg.msg_accrightslen = sizeof(int);
#endif

  msg.msg_name = NULL;
  msg.msg_namelen = 0;

  iov[0].iov_base = ptr;
  iov[0].iov_len = nbytes;

  msg.msg_iov = iov;
  msg.msg_iovlen = 1;

  /* 用recvmsg读取内容 */
  if( (n = recvmsg(fd, &msg, 0)) <= 0)
    return n;

/* 如果有辅助数据就取出来 */
#ifdef HAVE_MSGHDR_MSG_CONTROL
  if( (cmptr = CMSG_FIRSTHDR(&msg)) != NULL &&
     cmptr->cmsg_len == CMSG_LEN(sizeof(int))) {

    if(cmptr->cmsg_level != SOL_SOCKET)
      err_quit("control level != SOL_SOCKET");

    if(cmptr->cmsg_type != SCM_RIGHTS)
      err_quit("control type != SCM_RIGHTS");

    /* 把获得的辅助数据复值给recvfd */
    *recvfd = *(int*)CMSG_DATA(cmptr);
  } else {
    *recvfd = -1;
  }
#else
  if(msg.msg_accrightslen == sizeof(int))
    *recvfd = newfd;
  else
    *recvfd = -1;
#endif

  return n;
}
