CC = gcc
CFLAFS = -g

all: chess.o assistant.o
	${CC} ${CFLAFS} $^ -o chess

chess.o: chess.c
	${CC} -c chess.c -o chess.o  

assistant.o: assistant.c
	${CC} -c assistant.c -o assistant.o  

clean:
	del *.o *.exe
	