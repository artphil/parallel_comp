#ifndef __POOL__
#define __POOL__

#include <stddef.h>
#include <pthread.h>

// TAD do pool
struct pool;
typedef struct pool pool_t;

typedef void (*func_t)(void *arg);

// Construtor do pool
pool_t *create_pool(size_t size);

// Espera fim das tarefas
int wait_task(pool_t *trd);

// Adiciona tarefas a pool
int add_task(pool_t *trd, func_t func, void *arg);

// Destroi o pool
void destroy_pool(pool_t *trd);

#endif
