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
    float *data, sum, *sum_aux, aux;
    int to_print, to_time;
    int i, j, n_procs, rank, count;

    struct timeval start_time, end_time;

    // Le entrada
    scanf(" %s", option);
    printf("Lido : %s\n", option);
    scanf(" %d", &data_length);
    printf("Lido : %d\n", data_length);

    // Inicia programacao distribuida
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &n_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Cria vetor auxiliar
    count = (data_length / n_procs);
    if (data_length % n_procs != 0)
        count++;

    sum_aux = (float *)calloc(count, sizeof(float));

    j = 0;
    if (rank == 0)
    {
        // Le dados
        data = (float *)calloc(data_length, sizeof(float));
        for (i = 0; i < data_length; i++)
        {
            scanf(" %f", &data[i]);
            printf("Lido : %f\n", data[i]);
        }

        // Inicia a contagem do tempo
        gettimeofday(&start_time, NULL);

        // Envia dados a partir do processo zero
        for (i = 0; i < data_length; i++)
        {
            if (i % n_procs == 0)
                sum_aux[j++] = data[i];
            else
                MPI_Send(&data[i], 1, MPI_FLOAT, i % n_procs, 0, MPI_COMM_WORLD);
        }
    }
    else
    {
        // Recebe dados do processo zero
        for (i = 0; i < count; i++)
            MPI_Recv(&sum_aux[j++], 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    sum = reduce(sum_aux, count);   // Retirar essa funcao

    while (1)                       // Implementar arvore de reducao
    {
        break;
    }

    if (rank == 0)
    {
        for (i = 1; i < n_procs; i++)
        {
            MPI_Recv(&aux, 1, MPI_FLOAT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sum += aux;
        }
    }
    else
    {
        MPI_Send(&sum, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
    }

    if (rank == 0)
    {
        // Identifica de impressao
        to_print = to_time = 0;

        if (option[0] == 'a')
            to_print = to_time = 1;
        else if (option[0] == 't')
            to_time = 1;
        else if (option[0] == 's')
            to_print = 1;

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