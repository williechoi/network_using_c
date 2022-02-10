#include <stdio.h>
#include <unistd.h>
#include <signal.h>

/**
 * signal이 발생했을 때 호출할 함수들 (핸들러)
 */ 
void timeout(int sig)
{
    if(sig==SIGALRM)
        puts("Time out!");
    
    /**
     * 2초 간격으로 SIGALRM 시그널을 반복 발생시키기 위해 alarm 함수 호출
     * 즉, alarm(2)로 인해 2초마다 timeout 함수가 호출되는 구조임
     */ 
    alarm(2);
}

void keycontrol(int sig)
{
    if(sig==SIGINT){
        puts("Ctrl+C pressed!");
    }
}

int main(int argc, char *argv[])
{
    int i;

    /**
     * signal handler 등록
     */ 
    signal(SIGALRM, timeout);
    signal(SIGINT, keycontrol);

    /**
     * 2초 뒤에 SIGALRM 발생 예약
     */ 
    alarm(2);

    for(i=0; i<3; i++)
    {
        puts("wait...");

        /**
         * 시그널이 발생하면 sleep 함수의 호출로 블로킹 상태의 프로세스가 깨어나게 됨!
         */ 
        sleep(100);
    }

    return 0;
}