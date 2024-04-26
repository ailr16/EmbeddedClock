APP		:=	game
CC		:=	gcc
CCFLAGS := -Wall



$(APP) : main.c
	$(CC) -c main.c -o main.o $(CCFLAGS)
	$(CC) -c pointersHeader.c -o pointersHeader.o $(CCFLAGS)
	$(CC) main.o pointersHeader.o -o pointersTests