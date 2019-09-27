
COMPILER = clang++
COMPILE_FLAGS = --std=c++17 -Wpedantic -g
GTEST_LINK_FLAGS = -lpthread -lgtest_main -lgtest  -lpthread

all: rayson-info test

test: rayson-test
	./rayson-test

rayson-test: rayson.hpp rayson-test.cpp
	${COMPILER} ${COMPILE_FLAGS} ${GTEST_LINK_FLAGS} rayson-test.cpp -o rayson-test

rayson-info: rayson.hpp rayson-info.cpp
	${COMPILER} ${COMPILE_FLAGS} rayson-info.cpp -o rayson-info

clean:
	rm -f rayson-info rayson-test
