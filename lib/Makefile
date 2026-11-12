.PHONY: all clean

CXX ?= g++
CXXSTD ?= -std=c++20

HEADER_PATH ?= includes
SOURCE_PATH ?= src
TESTS_PATH ?= tests
MAIN_PATH ?= $(SOURCE_PATH)/main.cpp

GENERAL_FLAGS ?= $(CXXSTD) -O3 -Wall -Wextra -Wpedantic -pthread -lrt -lpigpio  
SANITIZER_FLAGS ?= -fsanitize=address -fsanitize=undefined -fsanitize=leak -fanalyzer 
DEBUG_FLAGS ?= $(CXXSTD) -O0 -g -lrt -lpigpio  

SRC := $(filter-out $(MAIN_PATH), $(wildcard $(SOURCE_PATH)/*.cpp))
INCLUDES := $(wildcard $(HEADER_PATH)/*.hpp)
OBJECTS = $(SRC) $(INCLUDES)

all: main

main: $(OBJECTS) $(MAIN_PATH)
	$(CXX) -I $(HEADER_PATH) -o $@ $^ $(GENERAL_FLAGS) 

sanitized: $(OBJECTS) $(MAIN_PATH)
	$(CXX) -I $(HEADER_PATH) -o $@ $^ $(GENERAL_FLAGS) $(SANITIZER_FLAGS) 

debug_sanitized: $(OBJECTS) $(MAIN_PATH)
	$(CXX) -I $(HEADER_PATH) -o $@ $^ $(SANITIZER_FLAGS) $(DEBUG_FLAGS) 

debug: $(OBJECTS) $(MAIN_PATH)
	$(CXX) -I $(HEADER_PATH) -o $@ $^ $(DEBUG_FLAGS)

clean:
	rm -f *.o main sanitized debug cli_tests