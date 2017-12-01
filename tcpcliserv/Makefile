# @Author: sunshine
# @Date:   2017-11-12T16:13:49+08:00
# @Last modified by:   sunshine
# @Last modified time: 2017-11-23T15:25:41+08:00



SERV = tcpservselect01

$(SERV):tcpservselect01.c
	gcc -g -Wall -o $@ $< -L ../ -lunp


.PHONY:clean all
clean:
	rm str_echo.o
