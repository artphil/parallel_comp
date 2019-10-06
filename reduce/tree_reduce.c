#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include <mpi.h>

int reduce(float *V, int size)
{
    float sum = 0;
    for (int i = 0; i < size; i++)
        sum += V[i];
    return sum;
}

int main(int argc, char **argv)
{
    int data_length;
    char option[5];
    float *data, sum;
    int to_print, to_time;
    int size, rank;

    struct timeval start_time, end_time;

    // Inicia programacao distribuida
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
    {
        // Le entrada
        scanf(" %s", option);
        printf("Lido : %s\n", option);
        scanf(" %d", &data_length);
        printf("Lido : %d\n", data_length);
        data = (float *)calloc(data_length, sizeof(float));
        for (int i = 0; i < data_length; i++)
        {
            scanf(" %f", &data[i]);
            printf("Lido : %f\n", data[i]);
        }

        // Identifica de impressao
        to_print = to_time = 0;

        if (option[0] == 'a')
            to_print = to_time = 1;
        else if (option[0] == 't')
            to_time = 1;
        else if (option[0] == 's')
            to_print = 1;

        // Inicia a contagem do tempo
        gettimeofday(&start_time, NULL);
    }

    // Executa soma
    sum = reduce(data, data_length);

    if (rank == 0)
    {
        // Encerra a contagem do tempo
        gettimeofday(&end_time, NULL);

        // Imprime os numeros do grupo
        if (to_print)
        {
            printf("%.2f \n", sum);
        }
        // Imprime o tempo
        if (to_time)
            printf("%.1f\n", ((end_time.tv_sec - start_time.tv_sec) * 1000000u + end_time.tv_usec - start_time.tv_usec) / 1e3);
    }

    // Finaliza programacao distribuida
    MPI_Finalize();
    return 0;
}