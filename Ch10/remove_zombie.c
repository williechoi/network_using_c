#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void read_childproc(int sig)
{
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if(WIFEXITED(status)){
        printf("Remove proc id %d \n", id);
        printf("Child Process sent %d \n", WEXITSTATUS(status));
    }
}

int main(int argc, char *argv[])
{
    pid_t pid;
    int i;

    /**
     * SIGCHLD에 대한 시그널 핸들러를 등록한다.
     * 자식 프로세스가 종료되면 위의 함수가 호출된다.
     * waitpid 함수로 인해 자식 프로세스는 좀비가 되지 않고 소멸한다.
     */ 
    struct sigaction act;
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);

    pid = fork();
    if(pid == 0) {
        puts("Analyzing Product Information From Amazon.com...");
        for(i = 0; i < 10; i++){
            putchar('.');
            sleep(1);
        }
        puts("Analyzing Done.");
        return 12;
    } else {
        printf("child proc id : %d \n", pid);
        pid = fork();
        if(pid == 0){
            puts("Uploading Analysis Info To Web Server...");
            for(i = 0; i < 20; i++){
                putchar('.');
                sleep(1);
            }
            puts("Upload Finished");
            exit(24);
        } else {
            printf("child proc id: %d \n", pid);
            puts("Waiting for Child Process to Finish...");

            /**
             * SIGCHLD 발생 대기를 위해 부모 프로세스를 5초간 15회 멈춰놓았다.
             * 시그널이 발생하면 부모 프로세스는 깨어나므로 실제 멈춤 시간은 더 적다.
             */ 
            for (i = 0; i < 15; i++) {
                sleep(5);
            }
        }
        return 0;

    }


}