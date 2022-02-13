#include <stdio.h>
#include <sys/uio.h>

int main(int argc, char *argv[])
{
    struct iovec vec[2];
    char buf1[] = "ABCDEFG";
    char buf2[] = "1234567";
    int str_len;

    /**
     * 첫번째 전송할 데이터가 저장된 위치와 크기 정보
     */ 
    vec[0].iov_base = buf1;
    vec[0].iov_len = 3;
    /**
     * 두번째 전송할 데이터가 저장된 위치와 크기 정보
     */ 
    vec[1].iov_base = buf2;
    vec[1].iov_len = 4;

    /**
     * writev 함수 전달인자가 1이므로 콘솔로 출력됨
     */ 
    str_len = writev(1, vec, 2);
    puts("");
    printf("Write bytes: %d \n", str_len);
    return 0;
}