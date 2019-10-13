#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include <mpi.h>

#define print_log 0 // Setar 1 para imprimir as etapas

void print_array(float *V, int size)
{
    for (int i = 0; i < size; i++)
        printf("%.2f ", (V)[i]);
    printf("\n");
}

int main(int argc, char **argv)
{
    int data_length;
    char option[5];
    float *data, sum, *sum_aux, aux;
    int to_print, to_time;
    int i, j, n_procs, rank, count;

    struct timeval start_time, end_time;

    // Inicia programacao distribuida
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &n_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (print_log)
        printf("Inicio do programa RANK: %d\n", rank);

    if (rank == 0)
    {
        // Inicia a contagem do tempo
        gettimeofday(&start_time, NULL);

        // Le entrada
        // Opcao de impressao
        scanf(" %s", option);
        if (print_log)
            printf("Lido : %s\n", option);

        // Tamanho da entrada
        scanf(" %d", &data_length);
        if (print_log)
            printf("Lido : %d\n", data_length);

        // Tamanho do vetor auxiliar
        count = (data_length / n_procs);
        /*
            Para facilitar a implementacao decidimos colocar todos os vetores do mesmo tamanho,
            assim quando o tamanho do vetor de entada nao eh multiplo do numero de processos
            o vetor auxiliar é tem tamanho (data_length / n_procs) + 1.
            Os campos execentes serao preenchidos co zero.
        */
        if (data_length % n_procs != 0)
            count++;

        // Cria vetor auxiliar
        sum_aux = (float *)calloc(count, sizeof(float));

        // Envia o tamanho do vetor auxiliar aos demais ranks
        for (i = 1; i < n_procs; i++)
        {
            MPI_Send(&count, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            if (print_log)
                printf("Enviado count=%d para %d\n", count, i);
        }
    }
    else
    {
        //  Recebe o tamanho do vetor auxiliar
        MPI_Recv(&count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (print_log)
            printf("Recebido count=%d de %d\n", count, 0);

        // Cria vetor auxiliar
        sum_aux = (float *)calloc(count, sizeof(float));
    }

    j = 0;
    sum = 0.0;
    if (rank == 0)
    {
        // Le dados para soma
        data = (float *)calloc(data_length, sizeof(float));
        for (i = 0; i < data_length; i++)
        {
            // Popula vetor
            scanf(" %f", &data[i]);
            if (print_log)
                printf("Lido : %f\n", data[i]);
        }

        if (print_log)
            print_array(data, data_length);

        // Distribui dados a partir do processo zero
        for (i = 0; i < count * n_procs; i++)
        {
            if (i % n_procs == 0)
            {
                // Coloca dado no rank 0
                sum_aux[j++] = data[i];
                if (print_log)
                    printf("Colocado %f em %d\n", data[i], 0);
            }
            else if (i < data_length)
            {
                // Envia dado para outro rank
                MPI_Send(&data[i], 1, MPI_FLOAT, i % n_procs, 0, MPI_COMM_WORLD);
                if (print_log)
                    printf("Enviado %f para %d\n", data[i], i % n_procs);
            }
            else
            {
                // Envia 0 caso o vetor de dados tenha acabado
                MPI_Send(&sum, 1, MPI_FLOAT, i % n_procs, 0, MPI_COMM_WORLD);
                if (print_log)
                    printf("Enviado %f para %d\n", sum, i % n_procs);
            }
        }
        // Libera memoria que nao sera mais utilizada
        free(data);
    }
    else
    {
        // Recebe dados do processo zero e coloca no vetor auxiliar
        for (i = 0; i < count; i++)
        {

            MPI_Recv(&sum_aux[j++], 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (print_log)
                printf("i %d Recebido %f de %d\n", i, sum_aux[j - 1], 0);
        }
    }

    if (print_log)
        print_array(sum_aux, count);

    // Implementada arvore de reducao em carrocel
    /*
        EX: 3 processos
        [0] = {1,2,3}
        [1] = {4,5,6}
        [2] = {7,8,9}

        [0] {3}=> [1]   
        [1] {6}=> [2]   
        [2] {9}=> [0] 

        [0] = {10,2}
        [1] = {7,5}
        [2] = {13,8}   

        [0] {2}=> [1]   
        [1] {5}=> [2]   
        [2] {8}=> [0] 
       
        [0] = {18}
        [1] = {9}
        [2] = {18}   
    */
    i = count - 1;
    while (i > 0)
    {
        if (print_log)
            printf("Rank %d Loop %d\n", rank, i);

        // Envia valor para o proximo vizinho
        if (rank == n_procs - 1)
            MPI_Send(&sum_aux[i], 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
        else
            MPI_Send(&sum_aux[i], 1, MPI_FLOAT, (rank + 1) % n_procs, 0, MPI_COMM_WORLD);

        if (print_log)
            printf("Enviado %f para %d\n", sum_aux[i], (rank + 1) % n_procs);

        // Envia valor do vizinho anterior
        if (rank == 0)
            MPI_Recv(&sum, 1, MPI_FLOAT, n_procs - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        else
            MPI_Recv(&sum, 1, MPI_FLOAT, (rank - 1) % n_procs, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (print_log)
            printf("Recebido %f de %d\n", sum, (rank - 1) % n_procs);

        sum_aux[0] += sum;

        i--;
    }

    sum = sum_aux[0];

    // Libera memoria que nao sera mais utilizada
    free(sum_aux);

    // Reducao paralela

    /* 
    EX:
    [0][1][2][3][4][5][6][7][8][9]
    
    Eliminando ranks impares
    Cada processo Impar envia seu valor para o vizinho anterior
    [0]<=[1] [2]<=[3] [4]<=[5] [6]<=[7] [8]<=[9]
    
    Eliminando multiplos de 2, 4, 8 ...
    Cada processo nao multiplo de 4 envia seu valor para o segundo vizinho anterior
    [0]<=[2] [4]<=[6] [8]
    
    Cada processo nao multiplo de 8 envia seu valor para o quarto vizinho anterior
    [0]<=[4] [8]
    
    Cada processo nao multiplo de 16 envia seu valor para o oitavo vizinho anterior
    [0]<=[8]
    
    ...
    */ 
    i = 1;
    while (1)
    {
        // processos ociosos terminam
        if (sum == 0)
            break;

        // Testa multiplicidade 
        if (rank % (i * 2) == 0)
        {
            if (rank < n_procs - i)
                MPI_Recv(&aux, 1, MPI_FLOAT, rank + i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sum += aux;
            if (print_log)
                printf("Rank %d Recebido %f de %d\n", rank, aux, rank + i);
        }
        else
        {
            MPI_Send(&sum, 1, MPI_FLOAT, rank - i, 0, MPI_COMM_WORLD);
            if (print_log)
                printf("Rank %d Enviado %f para %d\n", rank, sum, rank - i);
            sum = 0;
        }

        // Termina o rank 0
        i *= 2;
        if (i > n_procs)
            break;
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