#include <memory.h>
#include <pthread.h>
#include <unistd.h>

#include <iostream>

using namespace std;

#define NTHREADS 10

void *thread(void *arg) {
    int *n = (int *) arg;
    sleep(10 - *n);
    cout << "Thread " << (*n) << endl;
    delete n;
    return NULL;
}

int main(int argc, char **argv) {
    pthread_t threads[NTHREADS];
    pthread_attr_t attr;
    memset(&attr, 0, sizeof(attr));
    int i;
    for (i=0; i<NTHREADS; i++) {
        int *p = new int;
        *p = i;
        pthread_create(threads + i, &attr, thread, p);
    }
    void *rval;
    for (i=0; i<NTHREADS; i++) {
        pthread_join(threads[i], &rval);
        cout << "Joined thread " << i << endl;
    }
    return 0;
}