CXX		  := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb

VPATH := src/renderer

BIN		:= bin
SRC		:= src
VENDOR	:= vendor
LIB		:= lib

LIBRARIES	:= -lglfw -lGLEW -lGLU -lGL
EXECUTABLE	:= application


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES) 

clean:
	-rm $(BIN)/*
