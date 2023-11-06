#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

int main(void)
{
	int fd = open("my_new_file.txt", O_APPEND | O_WRONLY);// opens a new file
    if (fd < 0)
        printf("Error opening the file\n");
    dup2(fd, STDOUT_FILENO); // now the STDOUT_FILENO points to “my_new_file.txt”
    close(fd);              // now we can delete the fd from the file descrptor table
    printf("This message will be printed to the my_new_file.txt, instead of the console.\n");
	return 0;
}
