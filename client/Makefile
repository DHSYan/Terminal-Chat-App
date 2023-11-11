# Make file
#
#
EXEC = client

CC = gcc

FILES = main.c 

${EXEC}: ${FILES}
	${CC} ${FILES} -o ${EXEC}

.PHONY: clean

clean: 
	rm ${EXEC} *.o
