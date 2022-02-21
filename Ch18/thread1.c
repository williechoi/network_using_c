#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
void* thread_main(void *arg);

int main(int argc, char** argv)
{
    pthread_t t_id;
    int thread_param = 5;
    int i;
    int cnt = 10;

    /**
     * 별도의 실행 흐름을 구성하는 쓰레드 생성을 요청한다.
     * thread_main 함수 호출 시 인자로 thread_param의 주소값을 전달한다.
     */ 
    if(pthread_create(&t_id, NULL, thread_main, (void*)&thread_param)!=0)
    {
        puts("pthread_create() error");
        return -1;
    }
    for(i=0; i<cnt; i++)
    {
        sleep(1);
        puts("running main");
    }
    puts("end of main");
    return 0;
}

/**
 * arg로 전달되는 것은 
 * pthread_create 함수의 네번째 전달인자이다.
 */ 
void* thread_main(void *arg)
{
    int i;
    int cnt = *((int*)arg);
    for(i=0; i<cnt; i++)
    {
        sleep(1);
        puts("running thread");
    }
    return NULL;
}