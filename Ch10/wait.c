#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void error_handling(char* message);

int main(int argc, char *argv[])
{
    int status;
    
    /**
     * Create a child process
     */ 
    pid_t pid = fork();

    if (pid == 0) {
        /**
         * return문을 통한 종료
         */ 
        sleep(15);
        return 3;
    } else {
        printf("I am the parent process!\n");
        printf("Child PID: %d \n", pid);
        
        /**
         * Create another child process
         */ 
        pid = fork();
        if (pid == 0){
            /**
             * exit() 함수 호출을 통한 종료
             */ 
            sleep(10);
            exit(7);
        } else {
            printf("Child PID: %d \n", pid);

            /**
             * wait 함수 호출. 종료된 프로세스 정보는 status에 담김.
             * wait 함수가 호출된 시점에 종료된 자식 프로세스가 없다면 Blocking 상태에 놓이게 됨
             */ 
            wait(&status);
            
            /**
             * WIFEXITED: 자식 프로세스의 정상 종료 여부 확인. 
             * 정상 종료인 경우에 한해 WEXITSTATUS 호출하여 자식 프로세스가 전달한 값 출력
             */ 
            if(WIFEXITED(status)) {
                printf("Child send one: %d \n", WEXITSTATUS(status));
            }
            
            /**
             * wait 함수 호출. 종료된 프로세스 정보는 status에 담김.
             */ 
            wait (&status);
            if(WIFEXITED(status)) {
                printf("Child send two: %d \n", WEXITSTATUS(status));
            }

            sleep(30);
        }
    }
}