CC=gcc
CFLAGS=-I.

main: main.o sll.o
	${CC} -o main main.o sll.o
main.o: main.c sll.h
	${CC} -c main.c
sll.o: sll.c sll.h
	${CC} -c sll.c
clean:
	rm -f *.o *~