.phony: all clean

HEADER_PATH ?= includes
SOURCE_PATH ?= src
TESTS_PATH ?= tests
MAIN_PATH ?= $(SOURCE_PATH)/main.c

GENERAL_FLAGS ?= -O3 -Wall -Wextra -Wpedantic -pthread -lrt -lpigpio  
SANITIZER_FLAGS ?= -fsanitize=address -fsanitize=undefined -fsanitize=leak -fanalyzer 
DEBUG_FLAGS ?= -O0 -g -lrt -lpigpio  

SRC := $(filter-out $(MAIN_PATH), $(wildcard $(SOURCE_PATH)/*.c))
INCLUDES := $(wildcard $(HEADER_PATH)/*.h)
OBJECTS = $(SRC) $(INCLUDES)

all: main

main: $(OBJECTS) $(MAIN_PATH)
	cc -I $(HEADER_PATH) -o $@ $^ $(GENERAL_FLAGS) 

sanitized: $(OBJECTS) $(MAIN_PATH)
	cc -I $(HEADER_PATH) -o $@ $^ $(GENERAL_FLAGS) $(SANITIZER_FLAGS) 

debug_sanitized: $(OBJECTS) $(MAIN_PATH)
	cc -I $(HEADER_PATH) -o $@ $^ $(SANITIZER_FLAGS) $(DEBUG_FLAGS) 

debug: $(OBJECTS) $(MAIN_PATH)
	cc -I $(HEADER_PATH) -o $@ $^ $(DEBUG_FLAGS)

clean:
	rm -f *.o main sanitized debug cli_tests