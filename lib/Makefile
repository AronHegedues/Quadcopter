.phony: all tests clean

HEADER_PATH ?= includes
SOURCE_PATH ?= src
TESTS_PATH ?= tests
MAIN_PATH ?= $(SOURCE_PATH)/main.c

GENERAL_FLAGS ?= -O3 -Wall -Wextra -Wpedantic -pthread 
SANITIZER_FLAGS ?= -fsanitize=address -fsanitize=undefined -fsanitize=leak -fanalyzer
DEBUG_FLAGS ?= -O0 -g

SRC := $(filter-out $(MAIN_PATH), $(wildcard $(SOURCE_PATH)/*.c))
INCLUDES := $(wildcard $(HEADER_PATH)/*.h)
OBJECTS = $(SRC) $(INCLUDES)

all: main

main: $(OBJECTS) $(MAIN_PATH)
	cc $(GENERAL_FLAGS) -I $(HEADER_PATH) -o $@ $^ -lpigpio -lrt

sanitized: $(OBJECTS) $(MAIN_PATH)
	cc $(GENERAL_FLAGS) $(SANITIZER_FLAGS) -I $(HEADER_PATH) -o $@ $^

debug_sanitized: $(OBJECTS) $(MAIN_PATH)
	cc $(DEBUG_FLAGS) $(SANITIZER_FLAGS) -I $(HEADER_PATH) -o $@ $^

debug: $(OBJECTS) $(MAIN_PATH)
	cc $(DEBUG_FLAGS) -I $(HEADER_PATH) -o $@ $^

clean:
	rm -f *.o main sanitized debug cli_tests