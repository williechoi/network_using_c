#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 30

int main(int argc, char *argv[])
{
    fd_set reads, temps;
    int result, str_len;
    char buf[BUF_SIZE];
    struct timeval timeout;

    FD_ZERO(&reads);
    FD_SET(0, &reads); // 0은 표준 입출력 (콘솔)이다.

    timeout.tv_sec = 5;
    timeout.tv_usec = 5000;

    while(1)
    {
        /**
         * select 함수 호출이 끝나면 변화가 생긴 파일 디스크립터의 위치를 제외한
         * 나머지 위치의 비트는 0으로 초기화된다.
         * 따라서 원본의 유지를 위해서는 복사의 과정을 거쳐야 한다.
         */
        temps = reads;
        
        /**
         * select 함수를 호출하기 전에
         * 매번 timeval 구조체 변수의 초기화를 반복해야 한다.
         */ 
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        
        /**
         * 콘솔로부터 입력된 데이터가 있을 경우에는 0보다 큰 수가 반환되며,
         * 아무것도 없어 타임아웃이 발생할 경우 0이 반환된다.
         */ 
        result = select(1, &temps, 0, 0, &timeout);
        if(result==-1)
        {
            puts("select() error!");
            break;
        } else if (result == 0) {
            puts("Time-out!");
        } else {
            /**
             * select함수가 0보다 큰 수를 반환했을 때, 즉, 콘솔에 무엇인가가 입력되었을 때 실행되는 영역이다.
             * 변화가 생긴 파일 디스크립터가 표준 입력이 맞는지 확인하고,
             * 맞으면 표준입력으로부터 데이터를 얻어 콘솔로 데이터를 출력한다.
             */ 
            if(FD_ISSET(0, &temps))
            {
                str_len = read(0, buf, BUF_SIZE);
                buf[str_len] = 0;
                printf("message from console: %s", buf);
            }
        }
    }
    return 0;
}