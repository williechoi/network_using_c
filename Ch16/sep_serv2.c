#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

int main(int argc, char* argv[])
{
    int serv_sock, clnt_sock;
    FILE* readfp;
    FILE* writefp;

    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz;
    char buf[BUF_SIZE] = {0,};

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr));
    listen(serv_sock, 5);
    clnt_adr_sz = sizeof(clnt_adr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

    /**
     * fdopen을 통해 FILE 포인터를 생성하면서,
     * dup 함수호출의 반환값을 대상으로 FILE 포인터를 생성하였다.
     */ 
    readfp = fdopen(clnt_sock, "r");
    writefp = fdopen(dup(clnt_sock), "w");

    /**
     * 클라이언트로 문자열 데이터 전송 후, fflush 함수호출을 통해 전송을 마무리한다.
     */ 
    fputs("FROM SERVER: Hi~ client? \n", writefp);
    fputs("I love all of the world \n", writefp);
    fputs("You are all welcome! \n", writefp);

    fflush(writefp);

    /**
     * FILE 포인터로부터 얻어지는 파일 디스크립터를 활용하여 shutdown 함수를 호출하였다. 
     * Half-close가 진행되므로 클라이언트로 EOF가 전달된다.
     */ 
    shutdown(fileno(writefp), SHUT_WR);
    fclose(writefp);    // 사실은 fclose가 Half close가 아니고 완전 종료이다.

    /**
     * 클라이언트가 마지막으로 전송한 문자열이 도착하는지 확인한다.
     */ 
    fgets(buf, sizeof(buf), readfp);
    fputs(buf, stdout);
    fclose(readfp);
    return 0;

}