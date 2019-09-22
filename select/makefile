
CC = g++
CFLAGS = -Wall 
TFLAGS = -lpthread 
Vflags = valgrind --leak-check=full

SRCS = sequential_select.cpp parallel_select.cpp 
BINS = sequential_select parallel_select

all : ${BINS}

sequential_select : sequential_select.cpp
	@echo "Checking.."
	$(CC) $(CFLAGS) -g sequential_select.cpp -o sequential_select 

parallel_select : parallel_select.o thpool.o
	@echo "Checking.."
	$(CC) $(CFLAGS) $(TFLAGS) -g parallel_select.o thpool.o -o parallel_select 

parallel_select.o : parallel_select.cpp
	@echo "Checking.."
	$(CC) $(CFLAGS) $(TFLAGS) -c parallel_select.cpp

thpool.o : thpool.cpp
	@echo "Checking.."
	$(CC) $(CFLAGS) $(TFLAGS) -c thpool.cpp

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