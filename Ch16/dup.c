#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    int cfd1, cfd2;
    char str1[] = "Hi~ I am string 1!\n";
    char str2[] = "It's nice day~ I am string 2!\n";

    /**
     * 파일 디스크립터 1을 복사하였다.
     */ 
    cfd1 = dup(1);
    /**
     * 복사한 파일 디스크립터를 다시 복사하고, 정수값도 지정하였다. 
     * 이제 stdin은 총 3개이다 (1, 3, 7)
     */ 
    cfd2 = dup2(cfd1, 7);

    printf("fd1=%d, fd2=%d \n", cfd1, cfd2);
    write(cfd1, str1, sizeof(str1));
    write(cfd2, str2, sizeof(str2));


    close(cfd1);    // file descriptor 3을 닫았다. 
    close(cfd2);    // file descriptor 7을 닫았다. 여전히 1은 열려있다.
    write(1, str1, sizeof(str1));   
    close(1);       // 1을 닫았다. 이제 stdin의 파일 디스크립터는 남아있지 않다.
    write(1, str2, sizeof(str2));
    
    return 0;
}