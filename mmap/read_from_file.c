#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	if(argc < 2)
	{
		printf("One argument passed instead of two!\n");
		return 0;
	}

	char *file_data;
	struct stat file_info;
	
	int f = open(argv[1], O_RDONLY);

	if(fstat(f, &file_info))
	{
		printf("Error!\n");
		return 0;
	}

	file_data = (char *) mmap(0,file_info.st_size, PROT_READ, MAP_SHARED, f, 0);

	fwrite(file_data, file_info.st_size, sizeof(char), stdout);
	return 0;
}
