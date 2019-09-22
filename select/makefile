
CC = g++
CFLAGS = -g -Wall -lpthread 
Vflags = valgrind --leak-check=full

SRCS := $(wildcard *.cpp)
BINS := $(SRCS:%.cpp=%)

# If the first argument is "run"...
ifeq (run,$(firstword $(MAKECMDGOALS)))
  # use the rest as arguments for "run"
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  # ...and turn them into do-nothing targets
  $(eval $(RUN_ARGS):;@:)
endif

all : ${BINS}

% : %.cpp
	@echo "Checking.."
	$(CC) $^ -o $@ $(CFLAGS) 

run : 
	$(foreach b, ${BINS}, \
	./$b ${RUN_ARGS};)

val :
	$(Vflags) ./${RUN_ARGS}

tests :
	$(Vflags) ./${RUN_ARGS} < \* insert entry tests here*\

clean :
	rm -rvf *.o ${BINS}

.PHONY: all run clean val tests fresh