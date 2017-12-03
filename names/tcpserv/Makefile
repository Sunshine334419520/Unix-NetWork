# @Author: sunshine
# @Date:   2017-11-25T15:55:40+08:00
# @Email:  guang334419520@126.com
# @Filename: Makefile
# @Last modified by:   sunshine
# @Last modified time: 2017-11-25T16:49:23+08:00

OBJS = daytimetcpserv.o tcp_listen.o
RESULTE = daytimetcpserv
$(RESULTE):$(OBJS)
	gcc -o $@ $< -L ../../ -lunp

daytimetcpserv.o:daytimetcpserv.c
	gcc -c $<

tcp_listen.o:tcp_listen.c
	gcc -c $<

.PHONY:clean
clean:
	rm -rf $(OBJS)
