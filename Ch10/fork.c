#include <stdio.h>
#include <unistd.h>

int gval = 10;

int main(int argc, char *argv[])
{
    pid_t pid;
    int lval = 20;
    // gval = 11, lval = 25
    gval++, lval += 5;


    pid = fork();
    
    if (pid==0) {
        /**
         * Child process
         * gval = 13, lval = 27
         */ 
        gval += 2, lval += 2;
        
    } else {
        /**
        * Parent Process
        * gval = 9, lval = 23
        */ 
        gval -= 2, lval -= 2;
    }

    if (pid==0) {
        /**
         * Child process
         */ 
        printf("Child Proc: [%d, %d] \n", gval, lval);
    } else {
        /**
        * Parent Process
        */ 
        printf("Parent Proc: [%d, %d] \n", gval, lval);
    }

    return 0;

}