CC = g++
CFLAGS = -g -Wall -fopenmp

SRCS := $(wildcard *.cpp)
BINS := $(SRCS:%.cpp=%)

all : ${BINS}

% : %.cpp
	@echo "Checking.."
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rvf *.o ${BINS}

