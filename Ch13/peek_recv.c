#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#define BUF_SIZE 30
void error_handling(char* message);

int main(int argc, char* argv[])
{
    int acpt_sock, recv_sock;
    struct sockaddr_in recv_adr, acpt_adr;
    int str_len, state;
    socklen_t recv_adr_sz;
    char buf[BUF_SIZE];

    if(argc!=2)
    {
        printf(" Usage : %s <port> \n", argv[0]);
        exit;
    }

    acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&acpt_adr, 0, sizeof(acpt_adr));
    acpt_adr.sin_family = AF_INET;
    acpt_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    acpt_adr.sin_port = htons(atoi(argv[1]));

    if(bind(acpt_sock, (struct sockaddr*)&acpt_adr, sizeof(acpt_adr))==-1)
        error_handling("bind() error");

    listen(acpt_sock, 5);

    recv_adr_sz = sizeof(recv_adr);
    recv_sock = accept(acpt_sock, (struct sockaddr*)&recv_adr, &recv_adr_sz);

    while(1)
    {
        /**
         * recv 함수를 호출하면서 MSG_PEEK 옵션을 전달하였다.
         * MSG_DONTWAIT 옵션을 전달하여서 데이터가 존재하지 않아도 블로킹 되지 않도록 하였다.
         */ 
        str_len = recv(recv_sock, buf, sizeof(buf)-1, MSG_PEEK | MSG_DONTWAIT);
        if(str_len > 0)
            break;
    }

    buf[str_len] = 0;
    printf("Buffering %d bytes: %s \n", str_len, buf);

    /**
     * recv 함수를 다시 호출하였다.
     * 아무런 옵션도 설정하지 않았기 때문에 이번에 읽어들인 데이터는 입력버퍼에서 지워진다.
     */ 
    str_len = recv(recv_sock, buf, sizeof(buf)-1, 0);
    buf[str_len] = 0;
    printf("read again : %s \n", buf);
    close(recv_sock);
    close(acpt_sock);
    return 0;
}

void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}