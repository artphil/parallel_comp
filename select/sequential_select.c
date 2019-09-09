#include <stdio.h>
#include <stdlib.h>

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

    i = first-1;
    j = first + (rand() % (last-first));
	pivot = A[j];
    trade(A, j, last);
	
	for (j=first; j<last; j++)
    {
        if (A[j] <= pivot)
        {
            i++;
            trade(A,i,j);
        }
    }

    i++;
    trade(A,i,last);

    return i;
}

int select(int *A, int first, int last, int number)
{
    int q,k;

    if (first == last) return A[first];

    q = partition(A, first, last);

    k = q-first+1;

    if (number==k) return A[q];
    else if (number<k) return select(A, first, q-1, number);
    else return select(A, q+1, last, number-k);
}


int main (int argc, char **argv)
{


    return 0;
}