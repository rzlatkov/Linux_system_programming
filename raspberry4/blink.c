#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main()
{
	int fd;
	int i = 10;
	unsigned char buff[2] = {'0','1'};
	if ((fd = open("/sys/class/gpio/gpio23/value", O_RDWR)) == -1)
	{
		printf("Error opening the file!\n");
		return 0;
	}
	
	while (i > 0)
	{
		write(fd,buff,1);
		sleep(1);
		write(fd,buff+1,1);
		sleep(1);
		--i;
	}
	return 0;
}

