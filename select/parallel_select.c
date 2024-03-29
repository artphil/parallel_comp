#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "thread_pool.h"

typedef struct part part_t;
struct part
{
	int threads;
	int t_num;
	int *V;
	int start;
	int end;
	int length;

	int index;
	int pivot;

	int l_start;
	int l_end;
	int l_length;

	int r_start;
	int r_end;
	int r_length;
};

void print_array(int *V, int size)
{
	for (int i = 0; i < size; i++)
		printf("%d ", (V)[i]);
	printf("\n");
}

void print_subarray(int *V, int s, int e)
{
	for (int i = s; i <= e; i++)
		printf("%d ", (V)[i]);
	printf("\n");
}

void print_part(part_t *p)
{
	printf("######################################################\n");
	printf("thread: %d\n", p->t_num);
	printf("indice	: %d\n", p->index);
	printf("pivo	: %d\n", p->pivot);
	printf("------------------------------------------------------\n");
	printf("v		: ");
	print_subarray(p->V, p->start, p->end);
	printf("tamanho	: %d\n", p->length);
	printf("inicio	: %d\n", p->start);
	printf("fim		: %d\n", p->end);
	printf("------------------------------------------------------\n");
	printf("l tam	: %d\n", p->l_length);
	printf("l ini	: %d\n", p->l_start);
	printf("l fim	: %d\n", p->l_end);
	printf("------------------------------------------------------\n");
	printf("r tam	: %d\n", p->r_length);
	printf("r ini	: %d\n", p->r_start);
	printf("r fim	: %d\n", p->r_end);
	printf("######################################################\n");
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
	for (int i = 0; i < n; i++)
		swap(V, i, rand() % n);
}

void pprint(int *V, char name, int start, int end)
{
	printf("%c = ", name);
	for (int i = start; i < end; i++)
		printf("%d ", (V)[i]);
	printf("\n");
}

// counts number of inactive threads
int inactive_threads(part_t *tparts){
	int i = 0;
	for(int j = 0; j <tparts[0].threads; j++)
	{
			if(tparts[j].length == 0)
			{
				i++;
			}
	}
	return i;
}

// Active threads
int *active_threads(part_t *tparts){
	int *i = (int*)malloc(1 * sizeof(int));
	int i_size = 0;
	for(int j = 0; j <tparts[0].threads; j++)
	{
			if(tparts[j].length > 0)
			{
				i_size++;
				i = realloc(i, i_size * sizeof(int));
				i[i_size-1] = j;
			}
	}
	return i;
}

int find_answer_index(part_t *tparts){
	for(int j = 0; j <tparts[0].threads; j++)
	{
			if(tparts[j].length == 1)
			{
				return j;
			}
	}
	// ERRO -  nao achou indice de resposta
	return -1;
}
// Cria duas particoes com numeros maiores e menores que o pivor
// void partition(part_t *tparts)
void partition(void *data)
{
	part_t *tparts = (part_t *)data;
	int i, j;

	if (tparts->length == 0)
	{
		tparts->l_length = tparts->r_length = 0;
		return;
	}

	i = tparts->start - 1;
	for (j = tparts->start; j <= tparts->end; j++)
	{
		if (tparts->V[j] < tparts->pivot)
		{
			i++;
			swap(tparts->V, i, j);
		}
	}

	tparts->l_start = tparts->start;
	tparts->l_end = i;
	tparts->l_length = tparts->l_end - tparts->l_start + 1;

	tparts->r_start = i + 1;
	tparts->r_end = tparts->end;
	tparts->r_length = tparts->r_end - tparts->r_start + 1;

//	printf("-------partition------\n");
 //	print_part(tparts);
}

// Seleciona o iesimo numero do vetor
int rand_select(part_t *tparts, pool_t *thpool)
{
	int number;
	int index;
	int p_aux[tparts[0].threads];
	int pivot = 0;
	int length = 0;
	int l_length = 0;
	int afk_threads;
	int *active_threads_indexes;

	afk_threads = inactive_threads(tparts);
	active_threads_indexes = active_threads(tparts);

	for (number = 0; number < (tparts[0].threads - afk_threads); number++)
	{
		//printf("_________________________\n");
		//print_part(&tparts[active_threads_indexes[number]]);

	//	printf("length 1 = %d\n", length);
		length += tparts[active_threads_indexes[number]].length;
//		printf("length 2 = %d\n", length);

		if (tparts[active_threads_indexes[number]].length > 0)
		{
			if (tparts[active_threads_indexes[number]].l_length > 0)
			{
				index = rand() % tparts[active_threads_indexes[number]].l_length;
			//	printf("tparts[number].l_start + index = %d\n", tparts[active_threads_indexes[number]].l_start + index);
		//		printf("tparts[number].V[tparts[number].l_start + index] = %d\n", tparts[active_threads_indexes[number]].V[tparts[active_threads_indexes[number]].l_start + index]);
				p_aux[number] = tparts[active_threads_indexes[number]].V[tparts[active_threads_indexes[number]].l_start + index];
		//	 	printf("index: %d pivot: %d\n",index,pivot);
			}
			else if (tparts[active_threads_indexes[number]].r_length > 0)
			{

				index = rand() % tparts[active_threads_indexes[number]].r_length;
	//			printf("tparts[number].r_start + index = %d\n", tparts[active_threads_indexes[number]].r_start + index);
//				printf("tparts[number].V[tparts[number].r_start + index] = %d\n", tparts[active_threads_indexes[number]].V[tparts[active_threads_indexes[number]].r_start + index]);

				p_aux[number] = tparts[active_threads_indexes[number]].V[tparts[active_threads_indexes[number]].r_start + index];
		//		printf("index: %d pivot: %d\n",index,pivot);
			}
		}
	}


	// Stoṕ condition
	if (length == 1){
		int result_index;
		result_index = find_answer_index(tparts);
		if(result_index == -1) // ERRO, achou indice errado
			exit(0);
		else
			return tparts[result_index].V[tparts[result_index].start];
	}

//	printf("p_aux: ");
/*	for(int i = 0; i < (tparts[0].threads - afk_threads); i++)
	{
		printf("%d ",p_aux[i]);
	}*/
//	printf("\n");
	pivot = p_aux[rand() % (tparts[0].threads - afk_threads)];

	//printf("(tparts[0].threads - afk_threads): %d | pivo: %d | \n", (tparts[0].threads - afk_threads), pivot);


//	printf("SELECT: \n");

	for (number = 0; number < tparts[0].threads; number++)
	{
		tparts[number].pivot = pivot;
		// partition(&tparts[number]); // PARALELIZAR
		add_task(thpool, &partition, &tparts[number]);
	}

	wait_task(thpool);

	for (number = 0; number < tparts[0].threads; number++)
	{
		l_length += tparts[number].l_length;
	}

	if (l_length > tparts[0].index - 1)
	{
	  //printf("\nL\n");

		for (number = 0; number < tparts[0].threads; number++)
		{
			tparts[number].length = tparts[number].l_length;
			tparts[number].start = tparts[number].l_start;
			tparts[number].end = tparts[number].l_end;
		}
	  //print_array(tparts[0].V, tparts[0].length);
		return rand_select(tparts,thpool);
	}
	else
	{
	//  printf("\nR\n");

		index = tparts[0].index - l_length;
		for (number = 0; number < tparts[0].threads; number++)
		{

			//print_part(&tparts[number]);
			tparts[number].index = index;
			tparts[number].length = tparts[number].r_length;
			tparts[number].start = tparts[number].r_start;
			tparts[number].end = tparts[number].r_end;
			//printf("\nMUDOU\n");
		//	print_part(&tparts[number]);
		}

		return rand_select(tparts,thpool);
	}
}

int main(int argc, char **argv)
{
	int DATA_LENGTH;
	char OPTION;
	int TARGET;
	int N_THREADS;
	int *data;
	int number, i_aux, l_aux;
	char to_print, to_time;
	part_t *tparts;
	pool_t *thpool; //Pool de threads

	struct timeval start_time, end_time;

	// srand(time(NULL));

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
	to_print = to_time = 0;

	if (OPTION == 'a')
		to_print = to_time = 1;
	else if (OPTION == 't')
		to_time = 1;
	else if (OPTION == 'l')
		to_print = 1;

	// Inicia a contagem do tempo
	gettimeofday(&start_time, NULL);

	// Aloca memoria
	data = (int *)malloc(DATA_LENGTH * sizeof(int));
	tparts = (part_t *)malloc(N_THREADS * sizeof(part_t));

	// Inicializa pool de Threads
	thpool = create_pool(N_THREADS);

	// Inicia vetor com numeros aleatorios de 1 a OO
	i_aux = 0;
	for (number = 0; number < DATA_LENGTH; number++)
	{
		i_aux += 1 + (rand() % 10);
		data[number] = i_aux;
		// printf("%d ", i_aux);
	}

	// printf("\ndata: ");
	// print_array(data, DATA_LENGTH);
	// shuffle(data, DATA_LENGTH);

	// Inicialisa estrutura de dados
	l_aux = DATA_LENGTH / N_THREADS;

	// printf("MAIN:\n");
	for (number = 0; number < N_THREADS; number++)
	{
		tparts[number].t_num = number;

		i_aux = number * l_aux;

		tparts[number].threads = N_THREADS;
		tparts[number].index = TARGET;

		tparts[number].V = data;
		tparts[number].start = tparts[number].l_start = i_aux;
		tparts[number].end = tparts[number].l_end = i_aux + l_aux - 1;
		tparts[number].length = tparts[number].l_length = l_aux;

		if (number == N_THREADS - 1)
		{
			tparts[number].end = tparts[number].l_end = DATA_LENGTH - 1;
			tparts[number].length = tparts[number].l_length = DATA_LENGTH - i_aux;
		}
		// print_part(&(tparts[number]));
	}

	// Inicializa pool de Threads
	thpool = create_pool(N_THREADS);

	// Seleciona iesimo numero
	number = rand_select(tparts,thpool);
	// number = rand_select(data, 0, DATA_LENGTH - 1, TARGET);

	// Destroi pool
	destroy_pool(thpool);


	// Encerra a contagem do tempo
	gettimeofday(&end_time, NULL);

	// Imprime os numeros do grupo
	if (to_print)
	{
		for (i_aux = 0; i_aux < DATA_LENGTH; i_aux++)
			printf("%d ", data[i_aux]);
		printf("\n");

		printf("%d \n", number);
	}
	// Imprime o tempo
	if (to_time)
		printf("%.6f\n", ((end_time.tv_sec - start_time.tv_sec) * 1000000u + end_time.tv_usec - start_time.tv_usec) / 1e6);

	// Desaloca memoria
	free(tparts);
	free(data);
	return 0;
}
