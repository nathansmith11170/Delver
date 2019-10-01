OBJS = main.cpp

DEPS = MatrixGraph.h prototypes.h LTexture.h

CC = g++

COMPILER_FLAGS = -w -g -pedantic -std=c++17 $(shell sdl2-config --cflags)

LINKER_FLAGS = $(shell sdl2-config --libs)

OBJ_NAME = delver

clean:
	rm -f delver *.o

%.o: %.cpp $(DEPS)
	$(CC) Console.cpp init.cpp MazeGenerate.cpp $(COMPILER_FLAGS) -c $(LINKER_FLAGS)

all: $(OBJS) $(DEPS) Console.o init.o MazeGenerate.o
	$(CC) $(OBJS) $(COMPILER_FLAGS) -o $(OBJ_NAME) Console.o init.o MazeGenerate.o $(LINKER_FLAGS)  -lSDL2_image