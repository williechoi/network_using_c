#include <stdio.h>
#include <windows.h>
#include <process.h>
unsigned WINAPI ThreadFunc(void* arg);

int main(int argc, char** argv)
{
    HANDLE hThread;
    unsigned threadID;
    int param = 5;

    /**
     * Thread의 main함수로 ThreadFunc을, param의 주소값을 전달하여 스레드 생성을 요구한다.
     */ 
    hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, (void*)&param, 0, &threadID);
    if(hThread==0)
    {
        puts("_beginthreadex() error");
        return -1;
    }
    Sleep(3000);    // Sleep 함수는 1/1000 초 단위로 블로킹한다. 따라서 3초의 대기시간을 갖는다.
    puts("end of main");
    return 0;

}

/**
 * WINAPI는 윈도우 고유의 키워드이다. 
 */ 
unsigned WINAPI ThreadFunc(void* arg)
{
    int i;
    int cnt = *((int*)arg);

    for(i=0; i<cnt; i++)
    {
        Sleep(1000);
        puts("running thread");
    }
    return 0;
}