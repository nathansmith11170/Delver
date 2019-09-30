OBJS = main.cpp

CC = g++

COMPILER_FLAGS = -w -g -pedantic -std=c++17 $(shell sdl2-config --cflags)

LINKER_FLAGS = $(shell sdl2-config --libs)

OBJ_NAME = delver

all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) -o $(OBJ_NAME)  $(LINKER_FLAGS) 