/*************************************************************************
    > File Name: readn.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: 2017年11月07日 星期二 10时26分38秒
 ************************************************************************/
#include "unp/unp.h"

ssize_t
readn(int filedes, void* vptr, size_t n)
{
	size_t nleft;
	ssize_t nread;
	char* ptr;
	nleft = n;
	ptr = vptr;
	while(nleft > 0) {
		if( (nread = read(filedes, ptr, nleft)) < 0) {
			if(errno == EINTR)
				nread = 0;
			else
				return(-1);
		} else if(nread == 0)
			break;
		nleft -= nread;
		ptr += nread;
	}

	return (n-nleft);
}
