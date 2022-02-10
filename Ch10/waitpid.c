#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int status;
    pid_t pid = fork();

    if(pid == 0) {
        /**
         * 자식 프로세스의 종료를 늦추기 위해 
         * sleep 함수 호출
         */ 
        sleep(15);
        return 24;
    } else {
        /**
         * WNOHANG을 세 번째 인자로 전달하였으므로,
         * 종료된 자식 프로세스가 없다면 0을 반환한다.
         */ 
        while( ! waitpid(-1, &status, WNOHANG)){
            sleep(3); // 이를 통해 waitpid 함수가 블로킹되지 않았음을 알 수 있음.
            puts("sleep three sec.");
        }

        if(WIFEXITED(status)){
            printf("Child sent me %d \n", WEXITSTATUS(status));
        }
    }
    return 0;
}