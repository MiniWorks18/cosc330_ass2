COMPILER = gcc
CFLAGS = -Wall -pedantic -pthread

EXES = nsb

ALL: ${EXES}

nsb: nsb.c
	${COMPILER} nsb.c ${CFLAGS} -o nsb

clean: 
	rm -f *~ *.o ${EXES}