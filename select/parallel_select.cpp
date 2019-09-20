#include <cstdio>
#include <cstdlib>
#include <ctime>

// Troca dois elementos de lugar no vetor
void trade(int *V, int x, int y)
{
	int aux;

	if (x != y)
	{
		aux = V[x];
		V[x] = V[y];
		V[y] = aux;
	}
}

// Checa se numero eh maior ou menor que pivot e registra no vetor de bits
void set_bit(int *V, int *check_array, int pivot, int index, int floor)
{
	if (V[index] < pivot)
		check_array[index - floor] = 1;
}

// Cria vetor de zeros
int *new_array(int size)
{
	return (int *)calloc(size, sizeof(int));
}

// calcula vetor de somas
int bit_sum(int *check_array, int *sum_array, int size)
{
	for (size_t i = 1; i < size; i++)
	{
		sum_array[i] = check_array[i - 1] + check_array[i];
	}

	return sum_array[size - 1];
}

// Cria duas particoes com numeros maiores e menores que o pivor
int partition(int *A, int first, int last)
{
	int i, j, pivot;
	int *bit_L_array = new_array(last - first + 1);
	int *bit_R_array = new_array(last - first + 1);
	int *sum_array = new_array(last - first + 1);

	i = first - 1;
	j = first + (rand() % (last - first));
	pivot = A[j];
	trade(A, j, last);


	for (j = first; j < last; j++)
	{
		set_bit(A, bit_L_array, pivot, j, first); 			///// PARALELIZAR
		set_bit(A, bit_R_array, pivot, j, first); 			///// PARALELIZAR
		// if (A[j] <= pivot)
		// {
		// 	i++;
		// 	trade(A, i, j);
		// }
	}


	i++;
	trade(A, i, last);

	return i;
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
		OPTION = argv[4][0];
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

	// Inicia vetor com numeros aleatorios de 1 a N
	for (number = 0; number < DATA_LENGTH; number++)
	{
		aux = rand() % DATA_LENGTH;
		while (checked[aux])
			aux = rand() % DATA_LENGTH;

		checked[aux] = 1;
		data[number] = aux + 1;
	}

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