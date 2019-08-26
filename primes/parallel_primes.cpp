#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <omp.h>

int main(int argc, char **argv)
{
    long MAX_NUMBER;
    char OPTION;
    int N_THREADS, this_thread;
    char **all_numbers;
    long number, multiple;
    clock_t start_time, end_time;
    bool to_print;
    bool to_time;
    double wall_timer;
    double *t_timer;

    // Le argumentos
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

    // Inicia a contagem do tempo
    start_time = clock();

    // Cria vetores de numeros por threads
    all_numbers = (char **)calloc(N_THREADS, sizeof(char *));

    // Cria vetor de tempos por threads
    t_timer = (double *)calloc(N_THREADS, sizeof(double));
    wall_timer = omp_get_wtime();

    // Popula Vetores de numeros e tempos
    for (this_thread = 0; this_thread < N_THREADS; this_thread++)
    {
        all_numbers[this_thread] = (char *)calloc(MAX_NUMBER, sizeof(char));
        all_numbers[this_thread][0] = all_numbers[this_thread][1] = 1;

        t_timer[this_thread] = wall_timer;
    }

    // Executa algoritimo de Eraclito
#pragma omp parallel for num_threads(N_THREADS) private(this_thread, number, multiple) schedule(static, 1)
    for (number = 2; number < MAX_NUMBER / 2; number++)
    {
        this_thread = omp_get_thread_num();
        if (!all_numbers[this_thread][number])
        {
            for (multiple = number + number; multiple < MAX_NUMBER; multiple += number)
                all_numbers[this_thread][multiple] = 1;
        }
        // Imprime o balanco de carga 
        // printf("Thread:%d number:%lu time:%lf\n", this_thread, number, omp_get_wtime() - t_timer[this_thread]);
        // t_timer[this_thread] = omp_get_wtime();
    }

    // Colapsa os multi vetores
#pragma omp parallel for num_threads(N_THREADS) private(this_thread, number, multiple) schedule(static)
    for (number = 2; number < MAX_NUMBER; number++)
        for (this_thread = 1; this_thread < N_THREADS; this_thread++)
        {
            if (all_numbers[0][number])
                break;
            all_numbers[0][number] |= all_numbers[this_thread][number];
        }

    // Encerra a contagem do tempo
    end_time = clock();

    // Imprime os primos
    if (to_print)
    {
        for (number = 2; number < MAX_NUMBER; number++)
        {
            if (!all_numbers[0][number])
                printf("%lu ", number);
        }
        printf("\n");
    }
    // Imprime o tempo
    if (to_time)
        printf("%.6lf\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);

    return 0;
}