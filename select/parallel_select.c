#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include "thread_pool.h"

// ./parallel_select <N> <k> <type>

void array_print(int *V, int size)
{
	for (int i = 0; i < size; i++)
		printf("%d ", (V)[i]);
	printf("\n");
}

// Troca dois elementos de lugar no vetor
void swap(int *V, int x, int y)
{
	int aux;

	if (x != y)
	{
		aux = V[x];
		V[x] = V[y];
		V[y] = aux;
	}
}

void shuffle(int *V, int n)
{
	///printf("shuffle n: %d\n", n);
	for (int i = 0; i < n; i++)
		swap(V, i, rand() % n);
}

typedef struct t_data
{
	int *V;
	int *L;
	int *L_sum;
	int *R;
	int *R_sum;
	int pivot;
	int index;
	int start;
	int end;
} t_data;

void t_data_print(t_data *data)
{
	printf("ARG_BIT PRINTS: \n");
	printf("A: ");
	array_print(data->V, data->end + 1);
	printf("L: ");
	array_print(data->L, data->end + 1);
	printf("L_sum: ");
	array_print(data->L_sum, data->end + 1);
	printf("R: ");
	array_print(data->R, data->end + 1);
	printf("R_sum: ");
	array_print(data->R_sum, data->end + 1);
	printf("Pivot: %d\nIndex: %d\nStart: %d\nEnd: %d\n",
		   data->pivot,
		   data->index,
		   data->start,
		   data->end);
}

// Checa se numero eh maior ou menor que pivot e registra no vetor de bits
void set_bit(void *args)
{
	t_data *data = (t_data *)args;

	if (data->V[data->index] < data->pivot)
	{
		printf("L bit set on %d\n", data->index - data->start);
		data->L[data->index - data->start] = 1;
	}
	else if (data->V[data->index] > data->pivot)
	{
		printf("R bit set on %d\n", data->index - data->start);
		data->R[data->index - data->start] = 1;
	}
}

// Cria vetor de zeros
int *new_array(int size)
{
	return (int *)calloc(size, sizeof(int));
}

// calcula vetor de somas
int *soma_prefixos(t_data *data, int size)
{
	data->L_sum[0] = data->L[0];
	data->R_sum[0] = data->R[0];

	for (int i = 1; i < size; i++)
	{
		data->L_sum[i] = data->L_sum[i - 1] + data->L[i];
		data->R_sum[i] = data->R_sum[i - 1] + data->R[i];
	}
}

typedef struct particoes
{
	int *V;
	int *L;
	int *R;
	int *L_sum;
	int *R_sum;
	int *L_part;
	int *R_part;
	int L_size;
	int R_size;
	int side;
	int swap_index;
} particoes;

void struct_particoes_print(particoes *p)
{
	printf("Particoes:\nV: ");
	array_print(p->V, p->L_size + p->R_size + 1);
	printf("Bin L: ");
	array_print(p->L, p->L_size + p->R_size + 1);
	printf("Bin R: ");
	array_print(p->R, p->L_size + p->R_size + 1);
	printf("Sum L: ");
	array_print(p->L_sum, p->L_size + p->R_size + 1);
	printf("Sum R: ");
	array_print(p->R_sum, p->L_size + p->R_size + 1);
	printf("part L: ");
	array_print(p->L_part, p->L_size);
	printf("part R: ");
	array_print(p->R_part, p->R_size);
	printf("L_size: %d\n", p->L_size);
	printf("R_size: %d\n", p->R_size);
	printf("side: %d\n", p->side);
	printf("Swap_index: %d\n", p->swap_index);
}

particoes aloca_particao(t_data data, int size, int *L_part, int *R_part)
{
	int size_L = data.L_sum[size - 1];
	int size_R = data.R_sum[size - 1];

	particoes aux;

	aux.V = data.V;
	aux.L = data.L;
	aux.L_sum = data.L_sum;
	aux.R = data.R;
	aux.R_sum = data.R_sum;

	aux.L_size = size_L;
	aux.R_size = size_R;

	aux.L_part = L_part;
	aux.R_part = R_part;

	return aux;
}

void add_to_partition(void *arg)
{
	particoes *data = (particoes *)arg;
	if (data->side == 0)
	{ // Partition index from PrefixSum, value fromadd_to_partition original vector
		printf("colocando L_Part[%d]=%d\n", data->L_sum[data->swap_index] - 1, data->V[data->swap_index]);
		data->L_part[data->L_sum[data->swap_index] - 1] = data->V[data->swap_index];
	}
	else
	{
		printf("colocando R_Part[%d]=%d\n", data->R_sum[data->swap_index], data->V[data->swap_index]);
		data->R_part[data->R_sum[data->swap_index] - 1] = data->V[data->swap_index];
	}
}

// Cria duas particoes com numeros maiores e menores que o pivor
particoes *partition(int *A, int size, int result_index, int pivot, tpool_t *threads)
{
	int i, j;

	int *bit_L_array = new_array(size);
	int *bit_R_array = new_array(size);
	int *sum_L_array = new_array(size);
	int *sum_R_array = new_array(size);

	t_data *arg_bit = (t_data *)malloc(size * sizeof(t_data));

	/*
		1 - escolhe pivo
	*/
	///printf("size-1:%d\n", size - 1);
	///j = (rand() % size - 1);
	///printf("pivo: %d\n", pivot);
	///pivot = A[j];
	///printf("new pivo: %d\n", pivot);
	///swap(A, j, size - 1);
	array_print(A, size);
	/*
		2 - ADD jobs -> set valores no vetor binario
	*/
	int job;
	for (j = 0; j < size; j++)
	{
		arg_bit[j].V = A;

		arg_bit[j].L = bit_L_array;
		arg_bit[j].L_sum = sum_L_array;

		arg_bit[j].R = bit_R_array;
		arg_bit[j].R_sum = sum_R_array;

		arg_bit[j].index = j;
		arg_bit[j].pivot = pivot;

		arg_bit[j].start = 0;
		arg_bit[j].end = size - 1;

		//t_data_print(&(arg_bit[j]));

		// Adciona tarefa para a pool de threads
		threads->init = true;
		job = tpool_add_work(threads, &set_bit, (void *)&(arg_bit[j]));
		///printf("Job: %d\n", job);
	}

	// Espera todas as tarefas serem completadas
	int done = 0;
	for (;;)
	{
		done = tpool_wait(threads);
		printf("done %d\n", done);
		if (done == 42)
			break;
	}

	t_data_print(&(arg_bit[0]));

	/*
		3 - somar L e R
	*/
	soma_prefixos(&arg_bit[0], size);

	/*

		4 - ADD jobs
			-> Preenche vetores auxiliares -> if vetor bin L = 0 -> preenche A[i] no R, else no L.

	*/
	particoes *result = (particoes *)malloc(size * sizeof(particoes));
	int *L_part = (int *)calloc(arg_bit[0].L_sum[size - 1], sizeof(int));
	int *R_part = (int *)calloc(arg_bit[0].R_sum[size - 1], sizeof(int));

	for (int h = 0; h < size; h++)
	{
		result[h] = aloca_particao(arg_bit[0], size, L_part, R_part);
		result[h].swap_index = h;
	}

	for (int h = 0; h < size; h++)
	{

		if (result[h].L[h] == 1)
		{
			printf("making partition L: %d\n", result[h].swap_index);
			result[h].side = 0; // determina a particao (left = 0 or right = 1)
			threads->init = true;
			tpool_add_work(threads, &add_to_partition, &result[h]);
		}

		else if (result[h].R[h] == 1)
		{
			printf("making partition R: %d\n", result[h].swap_index);
			result[h].side = 1;
			threads->init = true;
			tpool_add_work(threads, &add_to_partition, &result[h]);
		}
	}

	int done2 = 0;
	for (;;)
	{
		done2 = tpool_wait(threads);
		printf("done2 %d\n", done2);
		if (done2 == 42)
		{
			break;
		}
	}

	printf("arg_bit[%d], Pivot encontrado %d: \n", 0, arg_bit[0].pivot);
	struct_particoes_print(&(result[0]));
	printf("\n");

	// TODO: REFATORAR E ADD FREEs
	/*
	for (int h = 0; h < size; h++)
	{
		free(&arg_bit[h]);
	}
	free(bit_L_array);
	free(bit_R_array);
	free(sum_L_array);
	free(sum_R_array);
	*/

	return result;
}

// Seleciona o iesimo numero do vetor
int rand_select(int *A, int size, int number, tpool_t *t)
{
	int q, k;
	particoes *parts;

	if (size == 1)
		return A[0];
	int x = rand() % size;
	int pivot = A[x];
	swap(A, x, size - 1);

	printf("Before partition:\nA: ");
	array_print(A, size);

	parts = partition(A, size, number, pivot, t);
	printf("Done parts\n");

	// k = q - first + 1;
	if (parts[0].L_size == number - 1)
		return pivot;

	else if (parts[0].L_size >= number)
	{
		printf("\nLEFT\n");
		A = parts[0].L_part;
		size = parts[0].L_size;
		free(parts);
		printf("Rand select:\nA: ");
		array_print(A, size);
		printf("size: %d\n", size);
		printf("k position: %d\n", number);
		return rand_select(A, size, number, t);
	}
	else
	{
		printf("\nRIGHT\n");
		A = parts[0].R_part;
		size = parts[0].R_size;
		int L_size = parts[0].L_size;
		free(parts);
		printf("Rand select:\nA: ");
		array_print(A, size);
		printf("size: %d\n", size);
		printf("k position: %d\n", number - L_size);
		return rand_select(A, size, number - L_size - 1, t);
	}
}

int comparer(const void *a, const void *b)
{
	return (*(int *)a - *(int *)b);
}

int main(int argc, char **argv)
{
	int DATA_LENGTH;
	char OPTION;
	int TARGET;
	int N_THREADS;
	int *data;
	char *checked;
	int number, aux;
	char to_print, to_time;
	clock_t start_time, end_time;
	tpool_t *thpool; //Pool de threads
	// Le argumentos
	if (argc < 5)
	{
		printf("Numero de agrumentos invalido\n");
		return 1;
	}
	else
	{
		DATA_LENGTH = atoi(argv[1]);
		///printf("1: %d\n", DATA_LENGTH);

		TARGET = atoi(argv[2]);
		///printf("2: %d\n", TARGET);

		OPTION = argv[3][0];
		///printf("3: %c\n", OPTION);

		N_THREADS = atoi(argv[4]);
		///printf("4: %d\n", N_THREADS);
	}

	///printf("Begin\n");
	// Identifica de impressao
	to_print = to_time = 0;

	if (OPTION == 'a')
		to_print = to_time = 1;
	else if (OPTION == 't')
		to_time = 1;
	else if (OPTION == 'l')
		to_print = 1;

	// Inicia a contagem do tempo
	start_time = clock();

	// Aloca memoria
	data = (int *)malloc(DATA_LENGTH * sizeof(int));

	// Inicializa pool de Threads
	thpool = tpool_create(N_THREADS);

	// Inicia vetor com numeros aleatorios de 1 a OO
	aux = 0;
	for (number = 0; number < DATA_LENGTH; number++)
	{
		aux += 1 + (rand() % 10);
		data[number] = aux;
	}
	shuffle(data, DATA_LENGTH);

	printf("Original:\nA: ");
	array_print(data, DATA_LENGTH);
	printf("size: %d\n", DATA_LENGTH);
	printf("k position: %d\n\n", TARGET);

	// Seleciona iesimo numero
	number = rand_select(data, DATA_LENGTH, TARGET, thpool);

	// Destroi pool
	tpool_destroy(thpool);

	// Encerra a contagem do tempo
	end_time = clock();

	// Imprime os numeros do grupo
	qsort(data, DATA_LENGTH, sizeof(int), comparer);
	if (to_print)
	{
		for (aux = 0; aux < DATA_LENGTH; aux++)
			printf("%d ", data[aux]);
		printf("\n");

		printf("%d \n", number);
	}
	// Imprime o tempo
	if (to_time)
		printf("%.6lf\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);

	return 0;
}
