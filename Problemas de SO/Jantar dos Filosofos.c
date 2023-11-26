#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

pthread_t *filosofo;
pthread_mutex_t *garfo;

void *jantar(void *var)
{
	int i = 0;
	while (!pthread_equal(*(filosofo + i), pthread_self()) && i < 5) i++;
	while (1)
	{
		printf("O filosofo %d está pensando.\n", i + 1);
		sleep(rand() % 10 + 5);
		int preferencia = i;
		int depois = (i + 1) % 5;
		pthread_mutex_lock(garfo + (preferencia > depois ? depois : preferencia));
		pthread_mutex_lock(garfo + (preferencia < depois ? depois : preferencia));
		printf("O filosofo %d está comendo.\n", i + 1);
		sleep(rand() % 10);
		pthread_mutex_unlock(garfo + preferencia);
		pthread_mutex_unlock(garfo + depois);
	};
}

int main()
{
	srand(time(NULL));
	filosofo = (pthread_t *)malloc(5 * sizeof(pthread_t));
	garfo = (pthread_mutex_t *)malloc(5 * sizeof(pthread_mutex_t));
	for (int i = 0; i < 5; ++i) if (pthread_mutex_init(garfo + i, NULL) != 0) printf("Erro nos garfos\n");
	for (int i = 0; i < 5; ++i)
	{
		int erro = pthread_create(filosofo + i, NULL, &jantar, NULL);
		if (erro != 0) printf("Erro nos filosofos.\n");
	};
	for (int i = 0; i < 5; ++i) pthread_join(*(filosofo + i), NULL);
	free(filosofo);
	free(garfo);
	return 0;
}
