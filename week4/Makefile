CC=gcc
CFLAGS=-I.

main: server.o sll.o
	${CC} -o client client.c
	${CC} -o server server.o sll.o
main.o: server.c sll.h
	${CC} -c server.c
sll.o: sll.c sll.h
	${CC} -c sll.c
clean:
	rm -f *.o *~