CC = gcc
CFLAGS = -Wall 
TFLAGS = -lpthread 
Vflags = valgrind --leak-check=full

SRCS = sequential_select.c parallel_select.c 
BINS = sequential_select parallel_select

# If the first argument is "run"...
ifeq (run,$(firstword $(MAKECMDGOALS)))
  # use the rest as arguments for "run"
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  # ...and turn them into do-nothing targets
  $(eval $(RUN_ARGS):;@:)
endif


all : ${BINS}

sequential_select : sequential_select.c
	@echo "Checking.."
	$(CC) $(CFLAGS) -g sequential_select.c -o sequential_select 

parallel_select : parallel_select.o thread_pool.o
	@echo "Checking.."
	$(CC) -g parallel_select.o thread_pool.o  $(CFLAGS) $(TFLAGS) -o parallel_select 
	rm parallel_select.o thread_pool.o
	
parallel_select.o : parallel_select.c
	@echo "Checking object.."
	$(CC) -c parallel_select.c

thread_pool.o : thread_pool.c
	@echo "Checking object.."
	$(CC) -c thread_pool.c

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