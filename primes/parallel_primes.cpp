#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <omp.h>

int main(int argc, char **argv)
{
    long MAX_NUMBER;
    char OPTION;
    int N_THREADS;
    long *all_numbers;
    long number, multiple;
    clock_t start_time, end_time;
    bool to_print;
    bool to_time;

    if (argc < 4)
    {
        printf("Numero de agrumentos invalido\n");
        return 1;
    }
    else
    {
        MAX_NUMBER = atoi(argv[1]) + 1;
        OPTION = argv[2][0];
        N_THREADS = atoi(argv[3]);
    }

    to_print = to_time = false;

    if (OPTION == 'a')
        to_print = to_time = true;
    else if (OPTION == 't')
        to_time = true;
    else if (OPTION == 'l')
        to_print = true;

    start_time = clock();

    all_numbers = (long *)calloc(MAX_NUMBER, sizeof(long));
    all_numbers[0] = all_numbers[1] = 1;

#pragma omp parallel for num_threads(N_THREADS) \
private(number, multiple) schedule(static, 1)
    for (number = 2; number < MAX_NUMBER; number++)
    {
        if (all_numbers[number] == 0)
        {
            // printf("T:%d n:%9lu ",  omp_get_thread_num(), number);
            for (multiple = number + number; multiple < MAX_NUMBER; multiple += number)
                all_numbers[multiple] = 1;
        }
    }

    end_time = clock();

    if (to_print)
    {
        for (number = 2; number < MAX_NUMBER; number++)
        {
            if (all_numbers[number] == 0)
                printf("%lu ", number);
        }
        printf("\n");
    }
    if (to_time)
        printf("%.6lf\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);

    return 0;
}