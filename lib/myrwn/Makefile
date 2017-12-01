CPP=gcc
FLAGS=-c -Wall -g
OBJ=readline.o readn.o writen.o
RESULT=myrw.a
$(RESULT):$(OBJ)
	ar rcs $@ $<
readline.o:readline.c
	$(CPP) $(FLAGS) $<
readn.o:readn.c
	$(CPP) $(FLAGS) $<
writen.o:writen.c
	$(CPP) $(FLAGS) $<
.PHONY:clean move
clean:
	rm $(OBJ)
move:
	sudo mv $(RESULT) /usr/lib/
 


