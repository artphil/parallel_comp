#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "thread_pool.h"

// Estrutura para receber as tarefas
struct task
{
	func_t func;
	void *arg;
	struct task *next;
};
typedef struct task task_t;

struct pool
{
	task_t *first;
	task_t *last;
	pthread_cond_t t_cond;
	pthread_cond_t t_do_cond;
	pthread_mutex_t t_mutex;
	size_t working_cnt;
	size_t thread_cnt;
	int init;
	int stop;
};

static task_t *create_task(func_t func, void *arg)
{
	task_t *work;

	if (func == NULL)
		return NULL;

	work = malloc(sizeof(*work));
	work->func = func;
	work->arg = arg;
	work->next = NULL;
	return work;
}

static void destroy_task(task_t *work)
{
	if (work == NULL)
		return;
	free(work);
}

static task_t *get_task(pool_t *trd)
{
	task_t *work;

	if (trd == NULL)
		return NULL;

	work = trd->first;
	if (work == NULL)
		return NULL;

	if (work->next == NULL)
	{
		trd->first = NULL;
		trd->last = NULL;
	}
	else
	{
		trd->first = work->next;
	}

	return work;
}

static void *task_maker(void *arg)
{
	pool_t *trd = arg;
	task_t *work;

	while (1)
	{
		pthread_mutex_lock(&(trd->t_mutex));
		if (trd->stop)
			break;

		if (trd->first == NULL)
			pthread_cond_wait(&(trd->t_cond), &(trd->t_mutex));

		work = get_task(trd);
		trd->working_cnt++;
		trd->init = 0;
		// printf("Subiu job %lu\n", trd->working_cnt);
		pthread_mutex_unlock(&(trd->t_mutex));

		if (work != NULL)
		{
			work->func(work->arg);
			destroy_task(work);
		}

		pthread_mutex_lock(&(trd->t_mutex));
		trd->working_cnt--;
		// printf("Desceu job %lu\n", trd->working_cnt);
		if (!trd->stop && trd->working_cnt == 0 && trd->first == NULL)
			pthread_cond_signal(&(trd->t_do_cond));
		pthread_mutex_unlock(&(trd->t_mutex));
	}

	trd->thread_cnt--;
	pthread_cond_signal(&(trd->t_do_cond));
	pthread_mutex_unlock(&(trd->t_mutex));
	return NULL;
}

int add_task(pool_t *trd, func_t func, void *arg)
{

	task_t *work;
	trd->init = 1;

	// printf("Criando Job\n");

	if (trd == NULL)
	{
		// printf("trd vazio\n");
		trd->init = 0;
		return 0;
	}

	work = create_task(func, arg);
	if (work == NULL)
	{
		// printf("work vazio\n");
		trd->init = 0;
		return 0;
	}

	pthread_mutex_lock(&(trd->t_mutex));
	if (trd->first == NULL)
	{
		trd->first = work;
		trd->last = trd->first;
	}
	else
	{
		trd->last->next = work;
		trd->last = work;
	}

	pthread_cond_broadcast(&(trd->t_cond));
	pthread_mutex_unlock(&(trd->t_mutex));

	return 1;
}

int wait_task(pool_t *trd)
{
	if (trd == NULL)
	{
		return 0;
		// printf("trd vazio - wait fail\n");
	}

	pthread_mutex_lock(&(trd->t_mutex));
	// printf("stop: %d, working_cnt: %lu, thread_cnt: %lu\n", trd->stop, trd->working_cnt, trd->thread_cnt);
	while (1)
	{
		// printf("stop: %d, working_cnt: %lu, thread_cnt: %lu\n", trd->stop, trd->working_cnt, trd->thread_cnt);
		if (trd->init)
		{
		// printf("... waiting init ...\n");
		}
		else
		if (trd->stop)
		{
			if (trd->thread_cnt == 0)
				break;
		}
		else
		{
			if (trd->working_cnt == 0)
				break;
		}
		pthread_cond_wait(&(trd->t_do_cond), &(trd->t_mutex));
	}
	pthread_mutex_unlock(&(trd->t_mutex));
	return 42;
}

pool_t *create_pool(size_t num)
{
	pool_t *trd;
	pthread_t thread;
	size_t i;

	if (num == 0)
		num = 2;

	trd = calloc(1, sizeof(*trd));
	trd->thread_cnt = num;

	pthread_mutex_init(&(trd->t_mutex), NULL);
	pthread_cond_init(&(trd->t_cond), NULL);
	pthread_cond_init(&(trd->t_do_cond), NULL);

	trd->first = NULL;
	trd->last = NULL;

	for (i = 0; i < num; i++)
	{
		pthread_create(&thread, NULL, task_maker, trd);
		pthread_detach(thread);
	}

	return trd;
}

void destroy_pool(pool_t *trd)
{
	task_t *work;
	task_t *work2;

	if (trd == NULL)
		return;

	pthread_mutex_lock(&(trd->t_mutex));
	work = trd->first;
	while (work != NULL)
	{
		work2 = work->next;
		destroy_task(work);
		work = work2;
	}
	trd->stop = 1;
	// printf("\n destroy trd->stop: %d\n", trd->stop);
	pthread_cond_broadcast(&(trd->t_cond));
	pthread_mutex_unlock(&(trd->t_mutex));

	wait_task(trd);

	pthread_mutex_destroy(&(trd->t_mutex));
	pthread_cond_destroy(&(trd->t_cond));
	pthread_cond_destroy(&(trd->t_do_cond));

	free(trd);
}
