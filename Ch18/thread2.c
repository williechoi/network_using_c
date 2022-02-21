#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

void* thread_main(void *arg);

int main(int argc, char** argv)
{
    pthread_t t_id;
    int thread_param = 5;
    void* thr_ret;

    if(pthread_create(&t_id, NULL, thread_main, (void*)&thread_param)!=0)
    {
        puts("pthread_create() error");
        return -1;
    }
    /**
     * pthread_join 함수를 호출하여, main함수가 t_id에 저장된
     * thread 종료시까지 대기하도록 한다.
     */ 
    if(pthread_join(t_id, &thr_ret)!=0)
    {
        puts("pthread_join() error");
        return -1;
    }

    printf("Thread return message: %s \n", (char*)thr_ret);
    free(thr_ret);
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
    char* msg = (char*)malloc(sizeof(char)*50);
    strcpy(msg, "Hello, I am thread! \n");

    for(i=0; i<cnt; i++)
    {
        sleep(1);
        puts("running thread");
    }
    return (void*)msg;
}