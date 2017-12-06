# @Author: sunshine
# @Date:   2017-12-06T18:21:24+08:00
# @Email:  guang334419520@126.com
# @Filename: Makefile
# @Last modified by:   sunshine
# @Last modified time: 2017-12-06T18:54:47+08:00


RESULT = mycat
OBJS = mycat.o myopen.o read_fd.o
CC = gcc
FLAGS = -Wall

$(RESULT) : $(OBJS)
	$(CC) $(FLAGS) -o  $@ $(OBJS) -lunp

mycat.o: mycat.c
	$(CC) $(FLAGS) -c $<

myopen.o: myopen.c
	$(CC) $(FLAGS) -c $<

read_fd.o: read_fd.c
	$(CC) $(FLAGS) -c $<

.PHONY: clean cleanall

clean:
	rm -rf $(OBJS)
cleanall:
	rm -rf $(OBJS) $(RESULT)
