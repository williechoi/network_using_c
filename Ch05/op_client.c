#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024
#define RLT_SIZE 4  // 피연산자의 바이트 수 상수화 (4바이트)
#define OPSZ 4      // 연산결과 바이트 수 상수화 (4바이트)
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int sock;
    char opmsg[BUF_SIZE];   // 데이터 송수신을 위한 메모리 공간은 배열로 생성하는 것이 좋음. 
    int result, opnd_cnt, i;
    struct sockaddr_in serv_adr;

    if(argc != 3){
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
        error_handling("connect() error!");
    else
        puts("Connected....");

    fputs("Operand count: ", stdout);
    
    /**
     * 유저로부터 피연산자의 (숫자) 개수정보를 입력받은 후, 이를 배열 opmsg에 저장하고 있음.
     * char 형으로의 변환은, 1바이트 정수 형태로 전달한다라고 정의한 프로토콜에 근거한 형변환
     * 즉, 1바이트로 표현 불가능한 범위의 정수가 입력되면 안됨.
     */ 
    scanf("%d", &opnd_cnt); 
    opmsg[0] = (char)opnd_cnt;

    for(i=0; i<opnd_cnt; i++)
    {
        /**
         * 프로그램 사용자로부터 정수를 입력 받아서 배열 opmsg에 이어서 저장하고 있음.
         * char형 배열에 4바이트 int형 정수를 저장해야 하기 때문에 int형 포인터로 형변환을 하는 모습.
         */ 
        printf("Operand %d: ", i+1);
        scanf("%d", (int*)&opmsg[i*OPSZ + 1]);
    }
    fgetc(stdin); // \n 개행문자 삭제를 위한 fgetc 함수 호출
    fputs("Operator: ", stdout);
    scanf("%c", &opmsg[opnd_cnt*OPSZ + 1]); // 연산자 정보를 입력받아서 배열 opmsg에 저장
    write(sock, opmsg, opnd_cnt*OPSZ + 2);
    read(sock, &result, RLT_SIZE); // 서버가 전달하는 연산결과를 저장. 수신할 데이터의 크기가 4바이트 (int)이므로 한번의 read함수 호출로 충분히 수신 가능.

    printf("Operation result: %d \n", result);
    close(sock);
    return 0;
}

void error_handling(char * message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}