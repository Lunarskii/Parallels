
#
#	Makefile options
#

MAKEFLAGS += -j16

#
#	Compilation and linking options
#

CXX							= g++
CXXFLAGS					= -I ./include -Wall -Werror -Wextra -std=c++17 -pedantic -g
LDFLAGS						= $(shell pkg-config --cflags --libs gtest) -lgtest_main
GCFLAGS						= -fprofile-arcs -ftest-coverage -fPIC
VGFLAGS						= --log-file="valgrind.txt" --track-origins=yes --trace-children=yes --leak-check=full --leak-resolution=med

#
#	Extensions
#

CPP							= .cc
TPP							= .tpp
HEADERS						= .h
OBJ							= .o

#
#	Libraries
#

COMMON_LIB					= lib.a

#
#	Connecting source file directories
#

VPATH						:= $(shell find . -type d)
vpath %$(CPP) $(VPATH)
vpath %$(HEADERS) $(VPATH)

#
#	Directories of source and object files
#

SRC_DIR						= sources/
OBJ_DIR						= ../obj/
SRC_ANT_DIR					= $(SRC_DIR)ant/
SRC_GAUSS_DIR				= $(SRC_DIR)gauss/
SRC_GRAPE_DIR				= $(SRC_DIR)grape/
SRC_COMMON_DIR				= $(SRC_DIR)common/
SRC_TESTS_DIR				= tests/
OBJ_ANT_DIR					:= $(subst $(SRC_DIR), $(OBJ_DIR), $(SRC_ANT_DIR))
OBJ_GAUSS_DIR				:= $(subst $(SRC_DIR), $(OBJ_DIR), $(SRC_GAUSS_DIR))
OBJ_GRAPE_DIR				:= $(subst $(SRC_DIR), $(OBJ_DIR), $(SRC_GRAPE_DIR))
OBJ_COMMON_DIR				:= $(subst $(SRC_DIR), $(OBJ_DIR), $(SRC_COMMON_DIR))
OBJ_TESTS_DIR				:= $(OBJ_DIR)$(SRC_TESTS_DIR)

#
#	Creating folders for object files
#

ALL_OBJ_DIRS				= $(OBJ_ANT_DIR) $(OBJ_GAUSS_DIR) $(OBJ_GRAPE_DIR) $(OBJ_TESTS_DIR) $(OBJ_COMMON_DIR)
FOLDERS						:= $(foreach dir, $(ALL_OBJ_DIRS), $(shell mkdir -p $(dir)))

#
#	Parsing of source files from directories and subdirectories
#

SRC_ANT						:= $(foreach dir, $(shell find $(SRC_ANT_DIR) -type d), $(wildcard $(dir)/*$(CPP)))
SRC_GAUSS					:= $(foreach dir, $(shell find $(SRC_GAUSS_DIR) -type d), $(wildcard $(dir)/*$(CPP)))
SRC_GRAPE					:= $(foreach dir, $(shell find $(SRC_GRAPE_DIR) -type d), $(wildcard $(dir)/*$(CPP)))
SRC_COMMON					:= $(foreach dir, $(shell find $(SRC_COMMON_DIR) -type d), $(wildcard $(dir)/*$(CPP)))
SRC_TESTS					:= $(foreach dir, $(shell find $(SRC_TESTS_DIR) -type d), $(wildcard $(dir)/*$(CPP)))

#
#	Creating object files
#

OBJ_ANT						:= $(patsubst %$(CPP), $(OBJ_ANT_DIR)%$(OBJ), $(notdir $(SRC_ANT)))
OBJ_GAUSS					:= $(patsubst %$(CPP), $(OBJ_GAUSS_DIR)%$(OBJ), $(notdir $(SRC_GAUSS)))
OBJ_GRAPE					:= $(patsubst %$(CPP), $(OBJ_GRAPE_DIR)%$(OBJ), $(notdir $(SRC_GRAPE)))
OBJ_COMMON					:= $(patsubst %$(CPP), $(OBJ_COMMON_DIR)%$(OBJ), $(notdir $(SRC_COMMON)))
OBJ_TESTS					:= $(patsubst %$(CPP), $(OBJ_TESTS_DIR)%$(OBJ), $(notdir $(SRC_TESTS)))

#
#	TARGETS
#

all: ant gauss grape

$(COMMON_LIB): $(OBJ_COMMON)
	ar rc $(COMMON_LIB) $(OBJ_COMMON)
	ranlib $(COMMON_LIB)

ant: $(COMMON_LIB) $(OBJ_ANT)
	$(CXX) $(CXXFLAGS) $(OBJ_ANT) -o ant $(COMMON_LIB)
	./ant

gauss: $(COMMON_LIB) $(OBJ_GAUSS)
	$(CXX) $(CXXFLAGS) $(OBJ_GAUSS) -o gauss $(COMMON_LIB)
	./gauss

grape: $(COMMON_LIB) $(OBJ_GRAPE)
	$(CXX) $(CXXFLAGS) $(OBJ_GRAPE) -o grape $(COMMON_LIB)
	./grape

test: $(COMMON_LIB) $(OBJ_TESTS) $(OBJ_ANT) $(OBJ_GAUSS) $(OBJ_GRAPE)
	$(CXX) $(CXXFLAGS) -O2 $(OBJ_TESTS) $(OBJ_DIR)ant/ant.o $(OBJ_DIR)ant/graph.o $(OBJ_DIR)gauss/gauss.o $(OBJ_DIR)grape/grape.o -o test $(COMMON_LIB) $(LDFLAGS)
	./test

valgrind: test
	valgrind $(VGFLAGS) ./test
	! grep -n "ERROR SUMMARY" valgrind.txt | grep -v "0 errors"

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(COMMON_LIB)
	rm -rf ant
	rm -rf gauss
	rm -rf grape
	rm -rf test
	rm -rf valgrind.txt
	rm -rf report
	rm -rf *.info
	rm -rf *.gc*

$(OBJ_ANT_DIR)%$(OBJ): %$(CPP)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_GAUSS_DIR)%$(OBJ): %$(CPP)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_GRAPE_DIR)%$(OBJ): %$(CPP)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_COMMON_DIR)%$(OBJ): %$(CPP)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_TESTS_DIR)%$(OBJ): %$(CPP)
	$(CXX) $(CXXFLAGS) -c $< -o $@

format_set:
	find . -iname "*$(CPP)" -o -iname "*$(HEADERS)" -o -iname "*$(TPP)" | xargs clang-format --style=google -i

format_check:
	find . -iname "*$(CPP)" -o -iname "*$(HEADERS)" -o -iname "*$(TPP)" | xargs clang-format --style=google -n

.PHONY: all test clean valgrind format_set format_check