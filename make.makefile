# MACROS

CC = GCC
CFLAGS = -Wall
SRC = main.c list.c list.h
OBJ = main.o list.o


# REGLAS EXPLÍCITAS

all: $(OBJ) 
   $(GCC) $(CFLAGS) -o main $(OBJ)


# REGLAS IMPLÍCITAS

funciones.o: funciones.c funciones.h
main.o: main.c funciones.h