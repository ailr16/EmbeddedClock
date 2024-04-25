APP		:=	game
CC		:=	gcc
CCFLAGS := -Wall



$(APP) : main.c
	$(CC) -c main.c -o main.o $(CCFLAGS)
	$(CC) -c bitsHeader.c -o bitsHeader.o $(CCFLAGS)
	$(CC) main.o bitsHeader.o -o bitsAndBytes