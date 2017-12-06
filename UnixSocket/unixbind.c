/**
 * @Author: sunshine
 * @Date:   2017-12-06T16:10:28+08:00
 * @Email:  guang334419520@126.com
 * @Filename: unixbind.c
 * @Last modified by:   sunshine
 * @Last modified time: 2017-12-06T16:25:00+08:00
 */

#include "unp/unp.h"

int main(int argc, char const *argv[]) {
  int sockfd;
  socklen_t len;
  struct sockaddr_un unixaddr1,unixaddr2;

  if(argc != 2)
    err_quit("usage: unixbind <pathname>");

  sockfd = Socket(AF_LOCAL, SOCK_STREAM, 0);

  unlink(argv[1]);

  bzero(&unixaddr1, sizeof(unixaddr1));

  unixaddr1.sun_family = AF_LOCAL;
  strncpy(unixaddr1.sun_path, argv[1], sizeof(unixaddr1.sun_path) - 1);

  Bind(sockfd, (SA*)&unixaddr1, SUN_LEN(&unixaddr1));

  len = sizeof(unixaddr2);
  Getsockname(sockfd, (SA*)&unixaddr2, &len);

  printf("bound name = %s, returned len = %d \n",
  unixaddr2.sun_path, len);
  return 0;
}
