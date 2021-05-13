#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char **argv)
{
	if(argc<2)
	{
		printf("One argument passed instead of two!\n");
		return 0;
	}
	
	char expression[100];
	sprintf(expression, "%s\n", argv[1]);
	int pc[2];
	int pp[2];
	pipe(pc);
	pipe(pp);

	pid_t process_id = fork();
	if(process_id == 0)
	{
		//child
		dup2(pp[0], STDIN_FILENO); //changing stdin
		dup2(pc[1], STDOUT_FILENO); //changing stdout
		execlp("bc", "bc", NULL);
	}
	else
	{
		//parent
		write(pp[1], expression, strlen(expression));
		char buffer[100];
		read(pc[0], buffer, 100);
		printf("%s\n", buffer);
	}
	return 0;
}
