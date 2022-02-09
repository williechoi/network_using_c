#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
void error_handling(char* message);

int main(int argc, char *argv[])
{
    int tcp_sock, udp_sock;
    int sock_type;
    socklen_t optlen;
    int state;

    optlen = sizeof(sock_type);

    /**
     * TCP, UDP socket을 각각 생성한다.
     */ 
    tcp_sock = socket(PF_INET, SOCK_STREAM, 0);
    udp_sock = socket(PF_INET, SOCK_DGRAM, 0);

    /**
     * TCP socket (SOCK_STREAM)과 UDP socket (SOCK_DGRAM)의 상수값을 각각 출력한다 (int type)
     */ 
    printf("SOCK_STREAM: %d \n", SOCK_STREAM);
    printf("SOCK_DGRAM: %d \n", SOCK_DGRAM);

    /**
     * socket의 타입 정보를 얻고 있다 (SO_TYPE).
     * 소켓의 타입 정보는 확인만 가능하고 변경이 불가능한 옵션이다.
     */ 
    state = getsockopt(tcp_sock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen);
    if(state)
        error_handling("getsockopt() error!");

    printf("Socket type one: %d \n", sock_type);

    state = getsockopt(udp_sock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen);
    if(state)
        error_handling("getsockopt() error!");

    printf("Socket type two: %d \n", sock_type);

    return 0;

}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}