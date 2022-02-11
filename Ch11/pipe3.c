#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define BUF_SIZE 127

int main(int argc, char *argv[])
{
    int parentToChild[2], childToParent[2];

    char childmsg[] = "Darth Vadar! I am here to put an end to You!";
    char parentmsg[] = "Ruth, I am your Father.";
    char buf[BUF_SIZE];

    pid_t pid;

    pipe(parentToChild), pipe(childToParent);
    pid = fork();

    if(pid == 0) {
        write(childToParent[1], childmsg, sizeof(childmsg));
        sleep(5);
        read(parentToChild[0], buf, BUF_SIZE);
        printf("Darth Vadar says: %s \n", buf);
    } else {
        
        read(childToParent[0], buf, BUF_SIZE);
        printf("Ruth Skywalker says: %s \n", buf);
        write(parentToChild[1], parentmsg, sizeof(parentmsg));
        sleep(8);
    }
    return 0;
}

