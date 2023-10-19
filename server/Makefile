# Make file
#
#
EXEC = server

CC = gcc

FILES = main.c auth.o TCPServer.o

${EXEC}: ${FILES}
	${CC} ${FILES} -o ${EXEC}
	echo "Output\n\n"
	./${EXEC}


auth.o: auth.c auth.h
	${CC} -c auth.c

TCPServer.o: TCPServer.h TCPServer.c
	${CC} -c TCPServer.c


.PHONY: clean

clean: 
	rm ${EXEC} *.o
