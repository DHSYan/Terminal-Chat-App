# Make file
#
#
EXEC = client

CC = gcc

FILES = main.c 

FLAGS = -pthread

${EXEC}: ${FILES}
	${CC} ${FILES} ${FLAGS} -o ${EXEC}

.PHONY: clean

clean: 
	rm ${EXEC} *.o
