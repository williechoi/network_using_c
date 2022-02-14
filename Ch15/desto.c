#include <stdio.h>
#include <fcntl.h>

int main(void)
{
    FILE *fp;
    int fd = open("data.dat", O_WRONLY | O_CREAT | O_TRUNC);    // file descriptor (int value) is returned
    if(fd == -1)
    {
        fputs("file open error", stdout);
        return -1;
    }

    fp = fdopen(fd, "w");   // file descriptor is turned into FILE pointer (write mode)
    fputs("Network C Programming \n", fp);  // use FILE POINTER to write text to file
    fclose(fp);                             // close file using FILE POINTER. no need to close file descriptor from this on.

    return 0;
}