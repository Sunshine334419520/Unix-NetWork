/**
 * @Author: sunshine
 * @Date:   2017-12-06T18:08:41+08:00
 * @Email:  guang334419520@126.com
 * @Filename: write_fd.c
 * @Last modified by:   sunshine
 * @Last modified time: 2017-12-06T18:21:08+08:00
 */

#include "unp/unp.h"

/**
 * 发送描述符
 * @param  fd     描述符
 * @param  ptr    缓冲区
 * @param  nbytes 缓冲区大小
 * @param  sendfd 需要发送的描述符
 * @return        发送的大小 ssize_t
 */
ssize_t write_fd(int fd, void* ptr, size_t nbytes, int sendfd)
{
  struct msghdr msg;
  struct iovec iov[1];

#ifdef HAVE_MSGHDR_MSG_CONTROL
  union {
    struct msghdr msg;
    struct control[CMSG_SPACE(sizeof(int))];
  } control_un;

  struct cmsghdr* cmptr;

  msg.msg_control = control_un.control;
  msg.msg_controllen = sizeof(control_un.control);

  /* set 需要发送的辅助data */
  cmptr = CMSG_FIRSTHDR(&msg);
  cmptr->cmsg_len = CMSG_LEN(sizeof(int));
  cmptr->cmsg_level = SOL_SOCKET;
  cmptr->cmsg_type = SCM_RIGHTS;
  *(int*)CMSG_DATA(cmptr ) = sendfd;
#else
  msg.msg_accrights = (caddr_t)&sendfd;
  msg.msg_accrightslen = sizeof(int);

#endif

  msg.msg_name = NULL;
  msg.msg_namelen = 0;

  iov[0].iov_base = ptr;
  iov[0].iov_len = nbytes;
  msg.msg_iov = iov;
  msg.msg_iovlen = 1;

  return (sendmsg(fd, &msg, 0));



}
