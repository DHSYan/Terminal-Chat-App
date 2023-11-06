# Make file


EXEC = server-refactor

CC = gcc

LIB = lib.h const.h

FILES = main.c client-handler.o

${EXEC}: ${FILES} ${LIB}
	${CC} ${FILES} -o ${EXEC}

# auth.o: auth.c auth.h ${LIB}
# 	${CC} -c auth.c
#
# comm.o: comm.c comm.h ${LIB}
# 	${CC} -c comm.c

client-handler.o: client-handler.c client-handler.h ${LIB}
	${CC} -c client-handler.c

.PHONY: clean

clean: 
	rm ${EXEC} *.o userlog.txt
