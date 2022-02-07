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
    int sd;
    FILE * fp;
    
    char buf[BUF_SIZE];
    char message[] = "Thank you for your service!\n";
    int read_cnt;
    struct sockaddr_in serv_adr;
    socklen_t clnt_adr_sz;

    if(argc != 3)
    {
        printf("Usage: %s <IP> <port> \n", argv[0]);
        exit(1);
    }

    /**
     * 서버가 전송하는 파일 데이터를 담기 위한 파일 하나를 생성한다.
     */ 
    fp = fopen("receive.dat", "wb");
    sd = socket(PF_INET, SOCK_STREAM, 0);

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if(connect(sd, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
        error_handling("connect() error");
    else
        puts("Connected!");

    /**
     * EOF가 전송될 때까지 데이터를 수신한 후
     * 아까 생성한 파일에 내용을 담는다.
     */ 
    while((read_cnt = read(sd, buf, BUF_SIZE))!=0)
        fwrite((void*)buf, 1, read_cnt, fp);

    puts("Received file data");
    /**
     * 서버에 인사 메시지를 전송한다.
     */ 
    write(sd, message, sizeof(message));
    fclose(fp);
    close(sd);

    return 0;
}

void error_handling(char * message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}