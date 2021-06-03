#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>

const int num_threads = 100; //Колко нишки отваряме
const int num_iterations = 10000; //Всяка нишка има за цел да итерира едно число 10000 пъти.

static int counter; //Това число ще итерираме
pthread_mutex_t lock; //Mutex/Ключалка с която ще позволяваме само на една нишка да работи с counter в даден момент
static void *mythread(void *arg) //Съдържанието на тази функция е нещото, което всяка нишка ще изпълнява
{
	int *c = arg;
	int i;
	for(i = 0;  i < num_iterations; i++) //Инкрементираме глобалното число 10000 пъти
	{
		usleep(10); //Симулираме някаква работа с приспиване 10 микросекунди
		pthread_mutex_lock(&lock); //Когато нишката си свърши работата заключва "c", което е указател към counter. Никой друг процес в операционната система не може да го пипа в този момент
		(*c)++; //След като се свърши работата инкрементираме числото
		pthread_mutex_unlock(&lock); //Когато инкрементира counter го отключвa
	}
	
	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t threads[num_threads]; //Създаваме масив от нишки, 100 на брой
	int i;

	if(argc != 1) //Ако сме сложили някакви console argument-и програмата ще излезе с грешка, не трябва да има конзолни аргументи.
	{
		return EXIT_FAILURE;
	}
	
	for(i = 0; i < num_threads; i++) //Създаваме 100 нишки, които ще изпълняват съдържанието на функция mythread и им се подава указател към counter
	{
		pthread_create(&threads[i], NULL, mythread, &counter);
	}

	for(i = 0; i < num_threads; i++) //Проверяваме за всяка нишка дали си е свършила работата. В момента в който всичките нишки са си свършили работата и са стигнали до края ще излезем от цикълa
	{
		pthread_join(threads[i], NULL);
	}

	printf("counter: Expected: %d, actual: %d\n", num_threads * num_iterations, counter); //Изписваме резултата от програмата. Броя нишки по броя итерации, които всяка нишка трябва да направи е Expected. В самия counter обаче се съхраняват реалния брой итерации, които са се случили

	return EXIT_SUCCESS;
}

