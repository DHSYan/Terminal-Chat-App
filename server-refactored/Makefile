# Make file


EXEC = server-refactor

CC = gcc

LIB = lib.h const.h

FILES = main.c client-handler.o protocol-processor.o string-util.o auth.o msgto.o user-util.o

${EXEC}: ${FILES} ${LIB}
	${CC} ${FILES} -o ${EXEC}

auth.o: auth.c auth.h ${LIB}
	${CC} -c auth.c
#
# comm.o: comm.c comm.h ${LIB}
# 	${CC} -c comm.c

client-handler.o: client-handler.c client-handler.h ${LIB}
	${CC} -c client-handler.c

protocol-processor.o: protocol-processor.c protocol-processor.h ${LIB}
	${CC} -c protocol-processor.c

string-util.o: string-util.c string-util.h ${LIB}
	${CC} -c string-util.c

msgto.o: msgto.c msgto.h ${LIB}
	${CC} -c msgto.c

user-util.o: user-util.c user-util.h ${LIB}
	${CC} -c user-util.c
.PHONY: clean

clean: 
	rm ${EXEC} *.o userlog.txt
