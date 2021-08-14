COMPILER = gcc
CFLAGS = -Wall -pedantic -pthread

EXES = sem_ops.o nsb

ALL: ${EXES}

sem_ops.o: sem_ops.h sem_ops.c
	${COMPILER} sem_ops.c ${CFLAGS} -c -o sem_ops.o
nsb: nsb.c sem_ops.o
	${COMPILER} nsb.c sem_ops.o ${CFLAGS} -o nsb


clean: 
	rm -f *~ *.o ${EXES}

run: 
	./nsb