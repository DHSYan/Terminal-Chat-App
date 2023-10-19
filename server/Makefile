# Make file
#
#
EXEC = server

CC = gcc

${EXEC}: auth.o main.c
	${CC} main.c auth.o -o server
	echo "Output\n\n"
	./server


auth.o: auth.c auth.h
	${CC} -c auth.c


.PHONY: clean

clean: 
	rm server *.o
