# Make file
#
#
EXEC = server

CC = gcc

FILES = main.c auth.o util.o interaction.o client_handler.o logging.o messaging.o group.o

${EXEC}: ${FILES}
	${CC} ${FILES} -o ${EXEC}

auth.o: auth.c auth.h
	${CC} -c auth.c

util.o: util.c util.h
	${CC} -c util.c

interaction.o: interaction.c interaction.h
	${CC} -c interaction.c

client_handler.o: client_handler.c client_handler.h
	${CC} -c client_handler.c

logging.o: logging.c logging.h
	${CC} -c logging.c

messaging.o: messaging.c messaging.h
	${CC} -c messaging.c

group.o: group.c group.h
	${CC} -c group.c

.PHONY: clean

clean: 
	rm ${EXEC} *.o userlog.txt
