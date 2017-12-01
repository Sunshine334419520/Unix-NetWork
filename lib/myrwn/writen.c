/*************************************************************************
    > File Name: writen.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: 2017年11月07日 星期二 10时37分48秒
 ************************************************************************/
#include "unp/unp.h"

ssize_t 
writen(int fd, const void* vptr, size_t n)
{
	size_t nleft;
	ssize_t nwritten;
	const char* ptr;
	ptr = vptr;
	nleft = n;
	while(nleft > 0) {
		if( (nwritten = write(fd, ptr, nleft)) <= 0) {
			if(errno == EINTR)
				nwritten = 0;
			else
				return -1;
		} 
		nleft -= nwritten;
		ptr += nwritten;
	}
	return n;
}
