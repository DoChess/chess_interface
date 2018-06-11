BINFOLDER := bin/
INCFOLDER := include/
OBJFOLDER := obj/
SRCFOLDER := source/
CREATE_OBJ_FOLDER := $(shell mkdir -p $(OBJFOLDER))

CC := g++
CFLAGS := -std=c++11 -Wall -ansi -Wshadow
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

SRCFILES := $(wildcard source/*.cpp)

all: $(SRCFILES:source/%.cpp=obj/%.o)
		$(CC) $(CFLAGS) $(LINKER_FLAGS) obj/*.o -o bin/finalBinary

obj/%.o: source/%.cpp
		$(CC) $(CFLAGS) $(LINKER_FLAGS) -c $< -o $@ -I./include

.PHONY: clean
clean:
		rm -rf obj/*
		rm -rf bin/*

run:
		cd bin/ && ./finalBinary
