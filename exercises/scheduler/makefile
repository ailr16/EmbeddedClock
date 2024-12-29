APP		:=	game
CC		:=	gcc
CCFLAGS := -Wall



$(APP) : main.c
	$(CC) -c main.c -o main.o $(CCFLAGS) -g
	$(CC) -c scheduler.c -o scheduler.o $(CCFLAGS)
	$(CC) main.o scheduler.o -o schedulerTests