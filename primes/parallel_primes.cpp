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
    double wall_timer;
    double *t_timer;

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

    wall_timer = omp_get_wtime();
    t_timer = (double *)calloc(N_THREADS, sizeof(double));
    for (int i; i < N_THREADS; i++)
        t_timer[i] = wall_timer;

#pragma omp parallel for num_threads(N_THREADS) private(number, multiple) schedule(static, 1)
    for (number = 2; number < MAX_NUMBER / 2; number++)
    {
        if (all_numbers[number] == 0)
        {
            // printf("T:%d n:%9lu ",  omp_get_thread_num(), number);
            // #pragma omp parallel for num_threads(N_THREADS) private(multiple) //schedule(static)
            // for (multiple = number+1; multiple < MAX_NUMBER; multiple++)
            //     if (multiple % number == 0)
            //         all_numbers[multiple] = 1;

            for (multiple = number + number; multiple < MAX_NUMBER; multiple += number)
                all_numbers[multiple]++;
        }
        printf("Thread:%d number:%lu time:%lf\n", omp_get_thread_num(), number, omp_get_wtime() - t_timer[omp_get_thread_num()]);
        t_timer[omp_get_thread_num()] = omp_get_wtime();
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