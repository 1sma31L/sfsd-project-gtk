#macros

PROG = main
CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0`
LIBS = `pkg-config --libs gtk+-3.0`

#targets: prerequisites
#	command
#   command
#   command

${PROG} : ${PROG}.c
	${CC} ${CFLAGS} -o ${PROG} ${PROG}.c ${LIBS}


clean :
	rm ${PROG}