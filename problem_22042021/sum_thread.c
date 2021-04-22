#include <pthread.h>
#include "calc_sum.h"

void* sum_thread(void *args)
{
	int res;
	int *array_args;
	array_args = args;

	res = calc_sum(array_args[0], array_args[1]);
	printf("Sum=%d\n", res);
	return NULL;
}
int main()
{
	int returned_res;
	int nums[2];

	fscanf(stdin, "%d %d", &nums[0], &nums[1]);
	
	pthread_t id;
	returned_res = pthread_create(&id,NULL,&sum_thread, nums);

	if(returned_res == 0)
	{
		printf("Thread created\n");
		pthread_join(id,NULL);
	}
	else
	{
		printf("Thread not created\n");
	}
	return 0;
}

