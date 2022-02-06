#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int sock;
    char message[BUF_SIZE];
    struct sockaddr_in my_adr, your_adr;
    int str_len, i;
    socklen_t adr_sz;

    if(argc != 2) {
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0); // UDP 소켓의 생성을 위해 socket함수의 두번째 인자로 SOCK_DGRAM 전달
    if(sock==-1)
        error_handling("UDP socket creation error");

    memset(&my_adr, 0, sizeof(my_adr));
    my_adr.sin_family = AF_INET;
    my_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    my_adr.sin_port = htons(atoi(argv[1]));

    if(bind(sock, (struct sockaddr*)&my_adr, sizeof(my_adr)) == -1)
        error_handling("bind() error");

    for(i=0; i<3; i++)
    {
        sleep(5);   // delay 5 seconds
        adr_sz = sizeof(your_adr);
        
        // bind에 할당된 주소로 전달되는 모든 데이터를 수신함. 데이터의 전달 대상에는 제한이 없음.
        str_len=recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr*)&your_adr, &adr_sz); 
        
        // 바로 위에서 데이터를 보낸 컴퓨터의 주소정보도 얻을 수 있으므로, 이 주소정보를 이용해서 수신한 데이터를 역으로 보냄
        printf("Message %d: %s \n", i+1, message);
    }
    close(sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}