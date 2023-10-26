# Make file
#
#
EXEC = server

CC = gcc

FILES = main.c auth.o TCPServer.o util.o

${EXEC}: ${FILES}
	${CC} ${FILES} -o ${EXEC}


auth.o: auth.c auth.h
	${CC} -c auth.c

TCPServer.o: TCPServer.h TCPServer.c
	${CC} -c TCPServer.c

util.o: util.c util.h
	${CC} -c util.c

.PHONY: clean

clean: 
	rm ${EXEC} *.o
