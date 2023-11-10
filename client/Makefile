# Make file
#
#
EXEC = client

CC = gcc

FILES = main.c connect-to-server.o interaction.o util.o

${EXEC}: ${FILES}
	${CC} ${FILES} -o ${EXEC}

connect-to-server.o: connect-to-server.c connect-to-server.h
	${CC} -c connect-to-server.c

interaction.o: interaction.c interaction.h
	${CC} -c interaction.c

util.o: util.c util.h
	${CC} -c util.c

.PHONY: clean

clean: 
	rm ${EXEC} *.o
