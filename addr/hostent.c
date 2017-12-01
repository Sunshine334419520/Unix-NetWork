/**
 * @Author: sunshine
 * @Date:   2017-11-22T17:01:21+08:00
 * @Email:  guang334419520@126.com
 * @Filename: hostent.c
 * @Last modified by:   sunshine
 * @Last modified time: 2017-11-25T17:20:03+08:00
 */



/*************************************************************************
    > File Name: hostent.c
    > Author: ma6174
    > Mail: ma6174@163.com
    > Created Time: 2017年11月22日 星期三 16时48分26秒
 ************************************************************************/

#include "../unp.h"
int
main(int argc, char** argv)
{
	char *ptr, **pptr;
	struct hostent* hptr;
	char str[INET_ADDRSTRLEN];

	while(--argc > 0) {
		ptr = *++argv;
		if((hptr = gethostbyname(ptr)) == NULL) {
			err_msg("gethostbyname error for host: %s: %s",
					ptr, hstrerror(h_errno));
			continue;
		}

		printf("offcial hostname: %s\n", hptr->h_name);

		for(pptr = hptr->h_aliases; *pptr != NULL; pptr++)
			printf("\tailas: %s\n",*pptr);

		switch(hptr->h_addrtype) {
		case AF_INET:
#ifdef AF_INET6
		case AF_INET6:
#endif
			pptr = hptr->h_addr_list;
			for(;*pptr != NULL; pptr++) {
				printf("\taddress: %s\n",
						Inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
			  if( (hptr = gethostbyaddr(*pptr, hptr->h_length,
					 hptr->h_addrtype)) == 0)
					 printf("\t(gethostbyaddr failed)\n");
				else if(hptr->h_name != NULL) {
					printf("\tname = %s\n",hptr->h_name);
				}
				else
					printf("\t(no hostname returned by gethostbyyaddr)\n");
			}
			break;
		default:
			err_ret("unknown addres type");
			break;
		}
	}
	exit(0);
}
