/*************************************************************************
    > File Name: readline.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: 2017年11月07日 星期二 10时43分11秒
 ************************************************************************/

#include "unp/unp.h"

static int read_cnt;
static char* read_ptr;
static char read_buff[MAXLINE];

static ssize_t
my_read(int fd, char* ptr)
{
	if(read_cnt <= 0) {
	again:
		if( (read_cnt = read(fd,read_buff,sizeof(read_buff))) < 0) {
			if(errno == EINTR)
				goto again;
			return -1;
		} else if(read_cnt == 0)
			return 0;
		read_ptr = read_buff;
	}

	read_cnt--;
	*ptr = *read_ptr++;
	return (1);
}

ssize_t
readline(int fd, void* vptr, size_t maxlen)
{
	ssize_t rc;
	size_t n;
	char c, *ptr;

	ptr = (char*)vptr;
	for(n = 1; n < maxlen; n++) {
		if( (rc = my_read(fd, &c)) == 1) {
			*ptr++ = c;
			if(c == '\n')
				break;
		}else if(rc == 0) {
			*ptr = 0;
			return (n - 1);
		}else 
			return -1;
	}
	*ptr = 0;
	return n;
}

ssize_t
readlinebuf(void** vptrptr)
{
	if(read_cnt)
		*vptrptr = read_ptr;
	return (read_cnt);
}
