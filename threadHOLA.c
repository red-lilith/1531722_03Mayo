#include <pthread.h>
#include <stdio.h>

#define NTHREADS 50

void * saludar(void * arg)
{
    printf("Hola %d\n", pthread_self());
    pthread_exit(NULL);
}

int main(int argc, char * argv[])
{
    pthread_t thread_id[NTHREADS];
    int i;
    for(i = 0; i < NTHREADS; i++){
        pthread_create(&thread_id[i], NULL, saludar, NULL);
    }

    pthread_exit(NULL);
}
