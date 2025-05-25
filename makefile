BINARY := CrowServer
CXX := g++
FLAGS := -Wall -Wextra -Werror -O0
SRC_DIR := ./src/
INC_DIR := ./inc/
OBJ_DIR := ./obj/
BIN_DIR := ./bin/
SOURCES := $(wildcard $(SRC_DIR)*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)%.cpp, $(OBJ_DIR)%.o, $(SOURCES))
EXECUTABLE := $(BIN_DIR)$(BINARY)

prep:
	mkdir bin csv dl download-archive obj

all: $(EXECUTABLE) prep

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	if [ ! -d $(OBJ_DIR) ]; then mkdir $(OBJ_DIR); fi
	$(CXX) $(FLAGS) -I$(INC_DIR) -c $< -o $@

$(EXECUTABLE): $(OBJECTS)
	if [ ! -d $(BIN_DIR) ]; then mkdir $(BIN_DIR); fi
	$(CXX) $(FLAGS) $^ -o $@

clean:                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
	rm -rf $(OBJ_DIR)* $(BIN_DIR)* ./dl/*


up: $(EXECUTABLE)
	$(BIN_DIR)$(BINARY)

re: clean all

.PHONY: all clean re prep