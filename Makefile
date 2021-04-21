SRC = lilac_chaser.c

CC = gcc

COMPILER_FLAGS = -Wall

LIBS=-I/usr/include/SDL2

LINKER_FLAGS = -lSDL2 -lpthread -lm -lSDL2_gfx

EXE_NAME = lilac_chaser

all : $(SRC)
	$(CC) $(SRC) $(LIBS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(EXE_NAME)
