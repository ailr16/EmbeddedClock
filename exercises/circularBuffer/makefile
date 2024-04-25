APP		:=	game
CC		:=	gcc
CCFLAGS := -Wall



$(APP) : main.c
	$(CC) -c main.c -o main.o $(CCFLAGS)
	$(CC) -c buffer.c -o buffer.o $(CCFLAGS)
	$(CC) main.o buffer.o -o bufferTests