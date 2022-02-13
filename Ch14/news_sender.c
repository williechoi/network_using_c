#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define TTL 64
#define BUF_SIZE 30

void error_handling(char * message);

int main(int argc, char* argv[])
{
    int send_sock;
    struct sockaddr_in mul_adr;
    int time_live = TTL;
    FILE *fp;
    char buf[BUF_SIZE];
    if(argc != 3) {
        printf("Usage : %s <GroupIP> <Port> \n", argv[0]);
        exit(1);
    }

    /**
     * 멀티캐스트 데이터의 송수신은 UDP 소켓을 대상으로 하기 때문에, 
     * 이 문장에서는 UDP 소켓을 생성하고 있다.
     */ 
    send_sock = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&mul_adr, 0, sizeof(mul_adr));

    /**
     * 데이터를 전송할 주소정보를 설정하고 있다. 이때, IP주소를 멀티캐스트 주소로 설정해야 하는 것이 중요하다.
     */ 
    mul_adr.sin_family = AF_INET;
    mul_adr.sin_addr.s_addr = inet_addr(argv[1]);   // Multicast IP
    mul_adr.sin_port = htons(atoi(argv[2]));        // Multicast Port

    /**
     * socket의 TTL 정보를 지정하고 있다.
     */ 
    setsockopt(send_sock, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&time_live, sizeof(time_live));
    if((fp=fopen("info.txt", "r"))==NULL)
        error_handling("fopen() error");

    /**
     * 실제 데이터를 전송하는 영역이다.
     * UDP 소켓을 기반으로 데이터 전송이 이루어지므로 sendto 함수를 이용한다.
     */ 
    while(!feof(fp))
    {
        fgets(buf, BUF_SIZE, fp);
        sendto(send_sock, buf, strlen(buf), 0, (struct sockaddr*)&mul_adr, sizeof(mul_adr));
        sleep(2);
    }
    fclose(fp);
    close(send_sock);
    return 0;

}

void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
