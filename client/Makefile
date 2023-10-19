# Make file
#
#
EXEC = client

CC = gcc

FILES = main.c

${EXEC}: ${FILES}
	${CC} ${FILES} -o server
	echo "Output\n\n"
	./${EXEC}


# auth.o: auth.c auth.h
# 	${CC} -c auth.c


.PHONY: clean

clean: 
	rm ${EXEC} *.o
