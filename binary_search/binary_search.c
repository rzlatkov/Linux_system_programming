#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

/*
The program searches a word in a wordlist. At first the program jumps to the middle
of the file. Second it looks for the closes newline characters on both sides. Third it
takes the string between them and compares it to the input string which we are looking for.
It compares each pair of strings char by char.
*/

struct word
{
	//size_t -> largest possible variable for the corresponding system/PC
	size_t start; //Position of the char after the leftside /n
	size_t end; //Position of the char before the rightside /n
	size_t len; //The size/len of the word
};
typedef struct word wd_t;

/*
The locateWord func extracts the word based on the /n chars on both sides.
wd_t *wd - pointer to the struct above.
char *file - pointer to the 1st char in the array representing the wordlist.
size_t pos - the index of the current char in the array.
*/
void locateWord(wd_t *wd, char *file, size_t pos);

// argc - the count of user arguments. the 1st arg is the executable name.
// argv - array where the user args are stored.
// ** - double pointer.
int main(int argc, char **argv)
{
	if(argc < 2)
	{
		printf("Error: minimum 2 arguments needed. 1st is the executable and 2nd is the word being searched.\n");
		return 0;
	}
	
	char *file_content; // the array containing the wordlist/file.
	/* 
	system struct taken from sys/stat.h
	Inside it the OS fills information about the file we are reading.
	The particular info we need is the file size. Without it we would need to
	iterate the file_content array which is not practical.
	*/
	struct stat file_stats; 
	/*
	fd - ID/file descriptor of the file created by the OS when opened.
	*/
	int fd = open("/usr/share/dict/cracklib-small", O_RDONLY);
	
	/*
	fstat is func from sys/stat.h lib. Extracts the metadata of the file.
	The data we need is the size of the file. 1 char = 1 byte
	*/
	if(fstat(fd, &file_stats)) //Extract metadata of the file into file_stats
	{
		printf("Error: No such file. Check the name again.");
		return 0;
	}
	
	// char * - casting. Tells the program to extract file data char by char.
	/*
	mmap - virtualizing/maps the real file to the virtual memory making it possible
	to be accessed by indexes (file_content array)
	1st arg - flag
	2nd arg - file size
	3rd arg - protect read (does not allow the program to write into the file_content).
	4th arg - informs other programs that this program is working with the file_content.
	5th arg - flag
	*/
	file_content = (char *) mmap(0, file_stats.st_size, PROT_READ, MAP_SHARED, fd, 0);

	size_t mid = file_stats.st_size / 2; //middle of the file content
	
	//possibleMin/Max the portion of the file_content we are searching into.
	size_t possibleMin = 0, possibleMax = file_stats.st_size - 1;
	
	wd_t wd; // creates worddata type struct.
	int ans; // stores the result of strncmp (string compare)
	char word[150]; //Stores the current word here.
	
	//If possibleMin is higher than possibleMax - word not found. Interval size is 0 or less.
	while(possibleMin < possibleMax)
	{
		locateWord(&wd, file_content, mid);
		strncpy(word, file_content + wd.start, wd.len); // string copy - copies the chars between the /n into "word" array.
		word[wd.len] = 0; // puts int 0 at the end of the word which indicates end of string or null terminator in ASCII.
		ans = strncmp(word, argv[1], wd.len);
		if(ans > 0) //The word is in the lower half of the current boundaries
		{
			possibleMax = wd.start-1;
			mid = (mid + possibleMin) / 2;
		}
		else if(ans < 0) //The word is in the upper half of the current boundaries
		{
			possibleMin = wd.end+1;
			mid = (mid + possibleMax) / 2;
		}
		else
		{
			printf("The word \"%s\" was found at positions [%ld] and [%ld]\n", argv[1], wd.start, wd.end-1);
			return 0;
		}
	}
	printf("The searched word \"%s\" not found\n", argv[1]);
	return 0;
}

void locateWord(wd_t *wd, char *file, size_t pos)
{
	char c = file[pos]; //Get the symbol in the middle
	int right = 0, left = 0; //Find how far left and right the word extends
	while(c != '\n') //If new line is found, then the end of the word is here
	{
		++right; //Move one position to the right
		c = file[pos + right];
	}
	wd->end = pos + right; // store pos + right into the end parameter in wd struct.
	
	do
	{
		++left; //Move one position to the left
		if(pos-left == 0) //If the beginning of the file is reached then this is the first word this is where it starts
		{
			left = pos + 1; //The word extends for the entire length of the file from the beginning to the current position
			break;
		}
		c = file[pos - left];
	}
	while(c != '\n');
	
	wd->start = pos - left + 1; //Since left is pointing to the new line at the beginning of the word, the first char in the word is left+1
	wd->len = wd->end - wd->start;
}
