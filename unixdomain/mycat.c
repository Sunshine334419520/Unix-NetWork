/**
 * @Author: sunshine
 * @Date:   2017-12-06T17:07:37+08:00
 * @Email:  guang334419520@126.com
 * @Filename: mycat.c
 * @Last modified by:   sunshine
 * @Last modified time: 2017-12-06T18:53:41+08:00
 */

#include "unp/unp.h"

int my_open(const char*, int);



int main(int argc, char const *argv[]) {
  int fd, n;

  /*参数必须带入有一个绝对路径*/
  if(argc != 2)
    err_quit("usage: mycat <pathname>");

  //调用我们自己的my_open函数来处理打开文件，不实用默认的open
  if( (fd = my_open(argv[1], O_RDONLY)) < 0)
    err_sys("canot open %s", argv[1]);

    char buff[BUFFSIZE];
  //读写
  while( (n = Read(fd, buff, BUFFSIZE)) > 0) {
    Write(STDOUT_FILENO, buff, n);
  }
  return 0;
}
