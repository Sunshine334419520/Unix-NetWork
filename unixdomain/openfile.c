/**
 * @Author: sunshine
 * @Date:   2017-12-06T18:04:06+08:00
 * @Email:  guang334419520@126.com
 * @Filename: openfile.c
 * @Last modified by:   sunshine
 * @Last modified time: 2017-12-06T18:08:23+08:00
 */


#include "unp/unp.h"

int main(int argc, char const *argv[]) {
  int fd;

  if(argc != 4)
    err_quit("openfile <sockfd#> <filename> <mode>");

  if( (fd = open(argv[2], atoi(argv[3]))) < 0) {
    exit( (errno > 0) ? errno : 255);

  }

  if(wirte_fd(atoi(argv[1], "", 1, fd)) < 0) {

    exit( (errno > 0) ? errno : 255);
  }
  
  return 0;
}
