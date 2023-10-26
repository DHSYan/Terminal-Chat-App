# Make file
#
#
EXEC = server

CC = gcc

FILES = main.c TCPServer.o

${EXEC}: ${FILES}
	${CC} ${FILES} -o ${EXEC}
	echo "Output\n\n"
	./${EXEC}
	make clean


# auth.o: auth.c auth.h
# 	${CC} -c auth.c

TCPServer.o: TCPServer.h TCPServer.c
	${CC} -c TCPServer.c


.PHONY: clean

clean: 
	rm ${EXEC} *.o
