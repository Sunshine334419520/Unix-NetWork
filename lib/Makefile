
libwrapsock.a:wrapsock.o
	ar rcs $@ $<

wrapsock.o:wrapsock.c
	gcc -c $< -lmyrw

.PHONY:clean move
clean:
	rm wrapsock.o
move:
	sudo mv libwrapsock.a /usr/lib/
