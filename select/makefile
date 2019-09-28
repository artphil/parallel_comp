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
	rm *.o

sequential_select : sequential_select.c
	@echo "Checking.."
	$(CC) $(CFLAGS) -g sequential_select.c -o sequential_select 

parallel_select : parallel_select.o thread_pool.o
	@echo "Checking.."
	$(CC) -g parallel_select.o thread_pool.o  $(CFLAGS) $(TFLAGS) -o parallel_select 
	
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


testes : sequential_select parallel_select 
	@echo "init tests:" > test.log
	@for number in 1000000 10000000 100000000 1000000000  ; do \
		echo "sequential_select $$number" ; \
		echo "sequential_select $$number" >> test.log; \
		./sequential_select $$number 555 t >> test.log; \
		./sequential_select $$number 555 t >> test.log; \
		./sequential_select $$number 555 t >> test.log; \
	done

	@for number in 1000000 10000000 100000000 1000000000 ; do \
		for threads in 2 4 8 ; do \
	    	echo "parallel_select $$number $$threads" ; \
	    	echo "parallel_select $$number $$threads" >> test.log; \
	    	./parallel_select $$number 555 t $$threads >> test.log; \
	    	./parallel_select $$number 555 t $$threads >> test.log; \
	    	./parallel_select $$number 555 t $$threads >> test.log; \
		done; \
	done

test_analise : sequential_select parallel_select 
	@echo "init tests:" > test_anl.log

	@for number in 10000 100000 1000000 10000000 100000000 1000000000 ; do \
		echo "parallel_select $$number 8" ; \
		echo "parallel_select $$number 8" >> test_anl.log; \
		./parallel_select $$number 555 t 8 >> test_anl.log; \
		./parallel_select $$number 555 t 8 >> test_anl.log; \
		./parallel_select $$number 555 t 8 >> test_anl.log; \
	done

	for threads in 1 2 3 4 5 6 7 8 ; do \
		echo "parallel_select 100000000 $$threads" ; \
		echo "parallel_select 100000000 $$threads" >> test_anl.log; \
		./parallel_select 100000000 555 t $$threads >> test_anl.log; \
		./parallel_select 100000000 555 t $$threads >> test_anl.log; \
		./parallel_select 100000000 555 t $$threads >> test_anl.log; \
	done

.PHONY: all run clean val tests testes fresh