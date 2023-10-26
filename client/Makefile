# Make file
#
#
EXEC = client

CC = gcc

FILES = main.c connect-to-server.o auth.o

${EXEC}: ${FILES}
	${CC} ${FILES} -o ${EXEC}
	echo "Output\n\n"
	make clean
	./${EXEC}


auth.o: auth.c auth.h
	${CC} -c auth.c

connect-to-server.o: connect-to-server.c connect-to-server.h
	${CC} -c connect-to-server.c

.PHONY: clean

clean: 
	rm ${EXEC} *.o
