#include <stdio.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <fcntl.h>

int main()
{
	/*
	struct winsize is included from termios.h library.
	It contains the x/y size of the terminal/bash.
	*/
	struct winsize ws;
	int fd = open("/dev/tty", O_RDONLY);
	
	/*
	Input-output control - sys call/func of the OS which takes the file descriptor
	of the process corresponding to openin the bash/terminal.
	TIOCGWINSZ - flag which tells the ioctl sys call to return ONLY the size of the terminal.
	It then stores in into ws struct.
	*/
	ioctl(fd, TIOCGWINSZ, &ws);
	printf("ws_row = %d\n", ws.ws_row); // rows count of the terminal.
	printf("ws_col = %d\n", ws.ws_col); // columns count of the terminal.
	// rows/cols depend on monitor res/inches/etc.
	return 0;
}
