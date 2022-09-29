CC = gcc
CFLAGS = -g -Wall
SRC = codigo_p0.c list.c list.h #tokens_list.c tokens_list.h
OBJ = codigo_p0.o list.o #tokens_list.o

all: $(OBJ)
	$(CC) $(CFLAGS) -o main $(OBJ)
	

funciones.o: list.c list.h #tokens_list.c #tokens_list.h
codigo_p0.o: codigo_p0.c list.h #tokens_list.h
