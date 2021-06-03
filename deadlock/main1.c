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

const int num_iterations = 100 * 100; //Всяка нишка има за цел да итерира едно число

static int counter; //това число ще итерира

static pthread_mutex_t counter_mutex; //Имаме един мютекс, който заключва самото итериране, като в предишната задача
static pthread_mutex_t print_mutex; //Имаме друг мютекс, който заключва изписването на екрана ако имаме кръгла итерация. Примерно 1000, 5000, 9000, 3000 и т.н. Причината да го заключваме е, за да може да се види нагледно кво ще стане ако единия mutex заключи едната нишка, другия mutex заключи другата нишка и за да се отключи първата нишка е нужно другата функция да си отключи мютекса, а за да може другата да си отключи мютекса е нужно първата да отключи нейния...

/* Counter thread. */
//Първата нишка ще инкрементира числото с тази функция
static void *mythread_counter(void *arg)
{
	int *c = arg;
	int i;

	//В този цикъл става инкрементирането
	for (i = 0;  i < num_iterations; i++) { 
		pthread_mutex_lock(&counter_mutex); //Заключваме самото инкрементиране
		(*c)++;
		if ((*c % 1000) == 0) //Проверяваме дали е кръгла итерация
		{
			pthread_mutex_lock(&print_mutex); //Ако е кръгла итерацията заключваме писането на екрана
			printf("Ding - a counter milestone.\n");
			pthread_mutex_unlock(&print_mutex); //Отключваме писането на екрана
		}
		pthread_mutex_unlock(&counter_mutex); //Отключваме инкрементирането
	}
	return NULL;
}

/* Aggressive counter reader. */
//Втората нишка ще инкрементира числото така
static void *mythread_printer(void *arg)
{
	int *c = arg;
	int i;

	for (i = 0;  i < num_iterations; i++) //инкрементираме числото в този цикъл
	{
		pthread_mutex_lock(&print_mutex); //Заключваме писането на екрана

		printf("==========: \n"); //Изписваме това
		pthread_mutex_lock(&counter_mutex); //Заключваме инкрементирането
		printf("%d\n", *c); //Изписваме броя на конкретния инкремент
		pthread_mutex_unlock(&counter_mutex); //Отключва инкрементирането
		pthread_mutex_unlock(&print_mutex); //Отключва писането на екрана
	}
	return NULL;
}

/*
 * Какво реално ще се случи в горните 2 функции
 * 1. Те работят едновременно и постоянно отключват и заключват и двата мютекса
 * 2. Проблемът е, че първата нишка ще заключи първия мютекс, после ще заключи И втория мютекс, ще си свърши работата, после ще отключи първо втория мютекс и накрая ще отключия първия
 * 3. Обаче в даден момент така ще се случи, че първата нишка ще заключи първия мютекс и точно когато иска да заключи втория мютекс, за да си продължи работата напред втората нишка ще я изпревари и ще го заключи тя. Така и двете нишки не могат да продължат и забиват, защото другата нишка я е изпреварила с някой от мютексите и я държи за заложник
 */

int main(int argc, char *argv[])
{
	pthread_t threads[2]; //Създаваме си масив с две нишки

	if (argc != 1)
		return EXIT_FAILURE;

	pthread_mutex_init(&counter_mutex, NULL);
	pthread_mutex_init(&print_mutex, NULL);

	pthread_create(&threads[0], NULL, mythread_counter, &counter); //Пускаме първата нишка, която ще инкрементира числото с първата функция отгоре 
	pthread_create(&threads[1], NULL, mythread_printer, &counter); //Пускаме втората нишка, която ще инкрементира числото с втората нишка отгоре

	pthread_join(threads[0], NULL); //Първата нишка като приключи продължаваме
	pthread_join(threads[1], NULL); //Втората нишка като приключи продължаваме

	printf("counter: Expected: %d, actual: %d\n", num_iterations, counter);

	return EXIT_SUCCESS;
} 

