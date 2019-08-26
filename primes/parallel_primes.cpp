#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <sys/time.h>

int main(int argc, char **argv)
{
	long MAX_NUMBER;
	char OPTION;
	int N_THREADS, this_thread;
	char *all_numbers;
	long number, multiple;
	bool to_print;
	bool to_time;
	bool print_threads;
	float t_timer;
	struct timeval start_time, end_time;

	// Le argumentos
	print_threads = to_print = to_time = false;

	if (argc < 4)
	{
		printf("Numero de agrumentos invalido\n");
		return 1;
	}
	else
	{
		MAX_NUMBER = atoi(argv[1]) + 1;
		OPTION = argv[2][0];
		N_THREADS = atoi(argv[3]);
		if (argc > 4) print_threads = true;
	}

	if (OPTION == 'a')
		to_print = to_time = true;
	else if (OPTION == 't')
		to_time = true;
	else if (OPTION == 'l')
		to_print = true;

	// Inicia a contagem do tempo
	gettimeofday(&start_time, NULL);

	// Inicia a contagem de tempos por threads
	t_timer = omp_get_wtime();

	all_numbers = (char *)calloc(MAX_NUMBER, sizeof(char));
	all_numbers[0] = all_numbers[1] = 1;

	// Executa algoritimo de Eraclito
#pragma omp parallel for num_threads(N_THREADS) default(none) private(number, multiple, t_timer, this_thread) shared(all_numbers, MAX_NUMBER, print_threads) \
	schedule(static, 1)
	for (number = 2; number < MAX_NUMBER / 2; number++)
	{
		this_thread = omp_get_thread_num();
		if (all_numbers[number] == 0)
		{
			for (multiple = number + number; multiple < MAX_NUMBER; multiple += number)
				if (all_numbers[multiple] == 0)
					all_numbers[multiple]++;
		}
		// Imprime o balanco de carga
		if (print_threads)
		{
		printf("Thread:%d number:%lu time:%f\n", this_thread, number, omp_get_wtime() - t_timer);
		t_timer = omp_get_wtime();
		}
	}

	// Encerra a contagem do tempo
	gettimeofday(&end_time, NULL);

	// Imprime os primos
	if (to_print)
	{
		for (number = 2; number < MAX_NUMBER; number++)
		{
			if (all_numbers[number] == 0)
				printf("%lu ", number);
		}
		printf("\n");
	}
	// Imprime o tempo
	if (to_time)
		printf("%.6f\n", ((end_time.tv_sec - start_time.tv_sec) * 1000000u + end_time.tv_usec - start_time.tv_usec) / 1e6);

	return 0;
}