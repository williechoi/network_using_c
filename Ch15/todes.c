#include <stdio.h>
#include <fcntl.h>

int main(void)
{
    FILE *fp;
    int fd = open("data.dat", O_WRONLY | O_CREAT | O_TRUNC);

    if(fd == -1)
    {
        fputs("file open error", stdout);
        return -1;
    }

    printf("First File Descriptor : %d \n", fd);    // print integer value of file descriptor
    fp = fdopen(fd, "w");                           // turn file descriptor into FILE pointer
    fputs("TCP IP Socket Programming \n", fp);

    printf("Second file descriptor: %d \n", fileno(fp));    // turn FILE pointer into file descriptor and print it

    fclose(fp);
    return 0;
}