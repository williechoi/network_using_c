#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define BUF_SIZE 30

void error_handling(char* message);

int main(int argc, char *argv[])
{
    int fds[2];
    char str[] = "Hello, World!";
    char buf[BUF_SIZE];

    pid_t pid;

    if(pipe(fds)==-1)
        error_handling("pipe() error");

    pid = fork();
    if (pid==0) {
        sleep(3);
        puts("Hi, I am a child process: wanna say something to my parent!");
        write(fds[1], str, sizeof(str));
    } else {
        printf("Hi, I am the parent process. I created a child called %d \n", pid);
        sleep(10);
        puts("I heard that my child is gonna say something to me!");
        read(fds[0], buf, BUF_SIZE);
        printf("Child told me:  %s \n", buf);
    }
    return 0;
}

void error_handling(char * message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}