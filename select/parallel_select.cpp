#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <pthread.h>
#include "thpool.h"

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
	for (int i = 0; i < n; i++)
		swap(V, i, rand() % n);
}

typedef 
struct t_data
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
}
t_data;


// Checa se numero eh maior ou menor que pivot e registra no vetor de bits
void set_bit(void *args)
{
	t_data *data = (t_data*) args;

	if (data->V[data->index] < data->pivot)
	{
		data->L[data->index - data->start] = 1;
	}
	else
	{
		data->L[data->index - data->start] = 1;
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
		data->L_sum[i] = data->L_sum[i-1] + data->L[i];
		data->R_sum[i] = data->R_sum[i-1] + data->R[i];
	}
}	


void pprint(int *V, char name, int start, int end)
{
	printf("%c = ", name);
	for (int i = start; i < end; i++)
		printf("%d ", (V)[i]);
	printf("\n");
}


typedef 
struct particoes
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
}
particoes;

particoes aloca_particao(t_data data, int size)
{
	int size_L = data.L_sum[size-1];
	int size_R = data.R_sum[size-1];

	particoes aux;

	aux.V = data.V;
	aux.L = data.L;
	aux.L_sum = data.L_sum;
	aux.R = data.R;
	aux.R_sum = data.R_sum;
	
	aux.L_size = size_L;
	aux.R_size = size_R;


    aux.L_part = (int *)malloc(size_L *sizeof(int));
    aux.R_part = (int *)malloc(size_R *sizeof(int));

	return aux;
	
}

void add_to_partition(void *arg)
{	
	particoes *data = (particoes*) arg;
	if(data->side == 0)
	{	// Partition index from PrefixSum, value from original vector
		data->L_part[ data->L_sum[ data->swap_index ] ] = data->V[ data->swap_index ]; 
	}
	else
	{
		data->R_part[ data->R_sum[ data->swap_index ] ] = data->V[ data->swap_index ];  
	}
}

// Cria duas particoes com numeros maiores e menores que o pivor
particoes *partition(int *A, int size, int result_index, threadpool* threads)
{
	int i, j, pivot;

	int *array_new = (int *)malloc(size * sizeof(int));

	int *bit_L_array = new_array(size);
	int *bit_R_array = new_array(size);
	int *sum_L_array = (int *)malloc(size * sizeof(int));
	int *sum_R_array = (int *)malloc(size * sizeof(int));

	t_data *arg_bit = (t_data *) malloc(size * sizeof(t_data));
	
	/*
		1 - escolhe pivo
	*/
	j = (rand() % size - 1);
	pivot = A[j];
	swap(A, j, size - 1);

		
	/*
		2 - ADD jobs -> set valores no vetor binario
	*/
	for (j = 0; j < size-1; j++) 					
	{
		arg_bit[j].V = A;
		
		arg_bit[j].L = bit_L_array;
		arg_bit[j].L_sum = sum_L_array;
		
		arg_bit[j].R = bit_R_array;
		arg_bit[j].R_sum = sum_R_array;

		arg_bit[j].index = j;
		arg_bit[j].pivot = pivot;
		
		arg_bit[j].start = 0;
		arg_bit[j].end = size-1;
		
		// Adciona tarefa para a pool de threads
		thpool_add_work(*threads, &set_bit, (void*) &(arg_bit[j]));		
	}
	
	// Espera todas as tarefas serem completadas
	thpool_wait(*threads);
	
	/*
	3 - somar L e R
	*/
	soma_prefixos(&arg_bit[0], size);


	/*

		4 - ADD jobs 
			-> Preenche vetores auxiliares -> if vetor bin L = 0 -> preenche A[i] no R, else no L.

	*/
	particoes *result = (particoes*)malloc(size*sizeof(particoes));
	
	for(int h = 0; h < size;h++)
	{
		result[h] = aloca_particao(arg_bit[0], size);
		result[h].swap_index = h;
	}
	
	for(int h = 0; h < size; h++)
	{
		if(result->L[h] == 1)
		{
			result[h].side = 0; // determina a particao (left = 0 or right = 1)
			thpool_add_work(*threads, &add_to_partition, (void *) result);
		}

		else if(result->R[h] == 1)
		{
			result[h].side = 1;
			thpool_add_work(*threads, &add_to_partition, (void *) result);
		}
	}
	
	

	// TODO: REFATORAR E ADD FREEs
	free(arg_bit);
	


	return result;
}

// Seleciona o iesimo numero do vetor
int select(int *A, int size, int number, threadpool* t)
{
	int q, k;
	particoes *parts;

	if (size == 1)
		return A[0];

	parts = partition(A, size, number, t);

	// k = q - first + 1;
	if (parts->L_size == number-1)
	 	return parts->R_part[0];
	
	else if (parts->L_size > number)
	{
		A = parts->L_part;
		size = parts->L_size;
		free(parts);
		return select(A, size, number, t);
	}
	else
	{
		A = parts->R_part;
		size = parts->R_size;
		free(parts);
		return select(A, size, number - parts->L_size - 1, t);
	}
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
	bool to_print, to_time;
	clock_t start_time, end_time;
	threadpool thpool;                     //Pool de threads
	// Le argumentos
	if (argc < 5)
	{
		printf("Numero de agrumentos invalido\n");
		return 1;
	}
	else
	{
		DATA_LENGTH = atoi(argv[1]);
		TARGET = atoi(argv[2]);
		OPTION = argv[3][0];
		N_THREADS = atoi(argv[4]);
	}

	// Identifica de impressao
	to_print = to_time = false;

	if (OPTION == 'a')
		to_print = to_time = true;
	else if (OPTION == 't')
		to_time = true;
	else if (OPTION == 'l')
		to_print = true;

	// Inicia a contagem do tempo
	start_time = clock();

	// Aloca memoria
	data = (int *)malloc(DATA_LENGTH * sizeof(int));
    
	// Inicializa pool de Threads
	thpool = thpool_init(N_THREADS);  

	// Inicia vetor com numeros aleatorios de 1 a OO
	aux = 0;
	for (number = 0; number < DATA_LENGTH; number++)
	{
		aux += 1 + (rand() % 10);
		data[number] = aux;
	}
	shuffle(data, DATA_LENGTH);

	// Seleciona iesimo numero
	number = select(data, DATA_LENGTH, TARGET, &thpool);

	// Encerra a contagem do tempo
	end_time = clock();

	// Imprime os numeros do grupo
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