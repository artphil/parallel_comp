#include <cstdio>
#include <cstdlib>

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

//cria duas particoes, uma maior outra menor que o pivor
int partition(int *A, int first, int last)
{
	int i, j, pivot;

	i = first - 1;
	j = first + (rand() % (last - first));
	pivot = A[j];
	trade(A, j, last);

	for (j = first; j < last; j++)
	{
		if (A[j] <= pivot)
		{
			i++;
			trade(A, i, j);
		}
	}

	i++;
	trade(A, i, last);

	return i;
}

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
	int TARGET;
	int *data;

	if (argc < 3)
	{
		printf("Numero de agrumentos invalido\n");
		return 1;
	}
	else
	{
		DATA_LENGTH = atoi(argv[1]);
		TARGET = atoi(argv[2]);
	}

	data = (int *)malloc(DATA_LENGTH * sizeof(int));

	return 0;
}