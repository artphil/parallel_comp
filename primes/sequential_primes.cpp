#include <cstdio>
#include <cstdlib>
#include <ctime>

int main(int argc, char **argv)
{
    long MAX_NUMBER;
    char OPTION;
    char *all_numbers;
    long *primes;
    long last_prime;
    long number, multiple;
    clock_t start_time, end_time;
    bool to_print;
    bool to_time;

    // Le argumentos
    if (argc < 4)
    {
        printf("Numero de agrumentos invalido\n");
        return 1;
    }
    else
    {
        MAX_NUMBER = atoi(argv[1]) + 1;
        OPTION = argv[2][0];
    }

    to_print = to_time = false;

    if (OPTION == 'a')
        to_print = to_time = true;
    else if (OPTION == 't')
        to_time = true;
    else if (OPTION == 'l')
        to_print = true;

    // Inicia a contagem do tempo
    start_time = clock();

    // Cria vetor de numeros
    all_numbers = (char *)calloc(MAX_NUMBER, sizeof(char));
    all_numbers[0] = all_numbers[1] = 1;

    // Cria vetor de primos
    primes = (long *)calloc(MAX_NUMBER, sizeof(long));
    last_prime = 0;

    // Executa algoritimo de Eraclito
    for (number = 2; number < MAX_NUMBER; number++)
    {
        if (all_numbers[number] == 0)
        {
            primes[last_prime++] = number;

            for (multiple = number; multiple < MAX_NUMBER; multiple += number)
                if (all_numbers[multiple] == 0) all_numbers[multiple] = 1;
        }
    }

    // Encerra a contagem do tempo
    end_time = clock();

    // Imprime os primos
    if (to_print)
    {
        for (number = 0; number < last_prime; number++)
        {
            printf("%lu ", primes[number]);
        }
        printf("\n");
    }
    // Imprime o tempo
    if (to_time)
        printf("%.6lf\n", (double)(end_time - start_time)/CLOCKS_PER_SEC);

    return 0;
}
