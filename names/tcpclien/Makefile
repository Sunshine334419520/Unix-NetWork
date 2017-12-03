# @Author: sunshine
# @Date:   2017-11-25T15:55:40+08:00
# @Email:  guang334419520@126.com
# @Filename: Makefile
# @Last modified by:   sunshine
# @Last modified time: 2017-11-25T16:02:40+08:00

OBJS = daytimetcpcli2.o tcp_connect.o
daytimetcpcli2:$(OBJS)
	gcc -o $@ $< -L ../../ -lunp

daytimetcpcli2.o:daytimetcpcli2.c
	gcc -c $<

tcp_connect.o:tcp_connect.c
	gcc -c $<

.PHONY:clean
clean:
	rm -rf $(OBJS)
