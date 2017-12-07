# @Author: sunshine
# @Date:   2017-12-06T18:21:24+08:00
# @Email:  guang334419520@126.com
# @Filename: Makefile
# @Last modified by:   sunshine
# @Last modified time: 2017-12-07T21:06:14+08:00


RESULT1 = mycat
RESULT2 = openfile
OBJS1 = mycat.o myopen.o read_fd.o
OBJS2 = openfile.o write_fd.o
CC = gcc
FLAGS = -Wall -g

$(RESULT1) : $(OBJS1) $(RESULT2)
	$(CC) $(FLAGS) -o  $@ $(OBJS1) -lunp

mycat.o: mycat.c
	$(CC) $(FLAGS) -c $<

myopen.o: myopen.c
	$(CC) $(FLAGS) -c $<

read_fd.o: read_fd.c
	$(CC) $(FLAGS) -c $<

$(RESULT2) : $(OBJS2)
	$(CC) $(FLAGS) -o $@ $(OBJS2) -lunp

openfile.o: openfile.c
	$(CC) $(FLAGS) -c $<

write_fd.o: write_fd.c
	$(CC) $(FLAGS) -c $<
.PHONY: clean cleanall all

clean:
	rm -rf $(OBJS1) $(OBJS2)
cleanall:
	rm -rf $(OBJS1) $(OBJS2) $(RESULT1) $(RESULT2)
