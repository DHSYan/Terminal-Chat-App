# Make file
#
#
EXEC = server

CC = gcc

FILES = main.c auth.o TCPServer.o util.o interaction.o client_handler.o logging.o

${EXEC}: ${FILES}
	${CC} ${FILES} -o ${EXEC}


auth.o: auth.c auth.h
	${CC} -c auth.c

TCPServer.o: TCPServer.h TCPServer.c
	${CC} -c TCPServer.c

util.o: util.c util.h
	${CC} -c util.c

interaction.o: interaction.c interaction.h
	${CC} -c interaction.c

client_handler.o: client_handler.c client_handler.h
	${CC} -c client_handler.c

logging.o: logging.c logging.h
	${CC} -c logging.c

.PHONY: clean

clean: 
	rm ${EXEC} *.o userlog.txt
