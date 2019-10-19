CC = mpicc
CFLAGS = -Wall 
TFLAGS = -lpthread 
Vflags = valgrind --leak-check=full

SRCS = tree_reduce.c sequential_reduce.c
BINS = tree_reduce sequential_reduce

# If the first argument is "run"...
ifeq (run,$(firstword $(MAKECMDGOALS)))
  # use the rest as arguments for "run"
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  # ...and turn them into do-nothing targets
  $(eval $(RUN_ARGS):;@:)
endif

all : ${BINS}

tree_reduce : tree_reduce.c
	@echo "Checking.."
	$(CC) $(CFLAGS) -g tree_reduce.c -o tree_reduce 

sequential_reduce : sequential_reduce.c
	@echo "Checking.."
	$(CC) $(CFLAGS) -g sequential_reduce.c -o sequential_reduce 


run : 
	$(foreach b, ${BINS}, \
	mpiexec -n 5 ./$b < input )

val :
	$(Vflags) ./${RUN_ARGS}

tests :
	$(Vflags) ./${RUN_ARGS} < \* insert entry tests here*\

clean :
	rm -rvf *.o ${BINS}


.PHONY: all run clean val tests fresh

