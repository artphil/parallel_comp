#include <cstdio>
#include <cstdlib>
#include <ctime>

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

// Checa se numero eh maior ou menor que pivot e registra no vetor de bits
void set_bit(int *V, int *L_array, int *R_array, int pivot, int index, int floor)
{
	if (V[index] < pivot)
	{
		L_array[index - floor] = 1;
	}
	else
	{
		R_array[index - floor] = 1;
	}
}

// Insere valor no novo vetor
void set_value(int *OLD, int *NEW, int old_index, int new_index)
{
	NEW[new_index] = OLD[old_index];
}

// Cria vetor de zeros
int *new_array(int size)
{
	return (int *)calloc(size, sizeof(int));
}

// calcula vetor de somas
int bit_sum(int *check_array, int *sum_array, int size)
{
	sum_array[0] = check_array[0];
	for (int i = 1; i < size; i++)
	{
		sum_array[i] = sum_array[i - 1] + check_array[i];
	}

	return sum_array[size - 1];
}

void pprint(int *V, char name, int start, int end)
{
	printf("%c = ", name);
	for (int i = start; i < end; i++)
		printf("%d ", (V)[i]);
	printf("\n");
}

// Cria duas particoes com numeros maiores e menores que o pivor
int partition(int *A, int first, int last)
{
	int i, j, pivot;
	int size = (last - first) + 1;

	int *array_new = (int *)malloc(size * sizeof(int));

	int *bit_L_array = new_array(size);
	int *bit_R_array = new_array(size);
	int *sum_L_array = (int *)malloc(size * sizeof(int));
	int *sum_R_array = (int *)malloc(size * sizeof(int));

	i = first - 1;
	j = first + (rand() % size - 1);
	pivot = A[j];
	swap(A, j, last);

	// pprint(A, 'A', first, last + 1);
	// pprint(bit_L_array, 'L', 0, size);
	// pprint(sum_L_array, 'l', 0, size);
	// pprint(bit_R_array, 'R', 0, size);
	// pprint(sum_R_array, 'r', 0, size);
	// printf("\n");
	// getchar();

	for (j = first; j < last; j++) 								///// PARALELIZAR
	{
		set_bit(A, bit_L_array, bit_R_array, pivot, j, first);
	}
																// Join
	i = bit_sum(bit_L_array, sum_L_array, size);
	bit_sum(bit_R_array, sum_R_array, size);

	for (j = first; j < last; j++) 								///// PARALELIZAR
	{
		if (bit_L_array[j - first])
			set_value(A, array_new, j, sum_L_array[j - first] - 1);

		else
			set_value(A, array_new, j, i + sum_R_array[j - first]);

		// printf("%2d %2d | %2d %2d | %2d %2d \n", 
		// j, 
		// A[j], 
		// bit_L_array[j - first], 
		// sum_L_array[j - first], 
		// bit_R_array[j - first], 
		// sum_R_array[j - first]);
	}

	set_value(A, array_new, last, i);

																		// Join
	for (j = first; j <= last; j++)
		A[j] = array_new[j - first];

	// pprint(A, 'A', first, last + 1);
	// pprint(bit_L_array, 'L', 0, size);
	// pprint(sum_L_array, 'l', 0, size);
	// pprint(bit_R_array, 'R', 0, size);
	// pprint(sum_R_array, 'r', 0, size);
	// printf("\ni = %d \n", i);
	// getchar();

	// free(array_new);
	// free(bit_L_array);
	// free(bit_R_array);
	// free(sum_L_array);
	// free(sum_R_array);

	return first+i;
}

// Seleciona o iesimo numero do vetor
int select(int *A, int first, int last, int number)
{
	int q, k;

	if (first == last)
		return A[first];

	q = partition(A, first, last);

	k = q - first + 1;

	if (number == k)
		return A[q];
	else if (number < k)
		return select(A, first, q - 1, number);
	else
		return select(A, q + 1, last, number - k);
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
	checked = (char *)malloc(DATA_LENGTH * sizeof(char));

	// Inicia vetor com numeros aleatorios de 1 a OO
	aux = 0;
	for (number = 0; number < DATA_LENGTH; number++)
	{
		aux += 1 + (rand() % 10);
		data[number] = aux;
	}
	shuffle(data, DATA_LENGTH);
	// libera memoria não utilizada
	free(checked);

	// Seleciona iesimo numero
	number = select(data, 0, DATA_LENGTH - 1, TARGET);

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