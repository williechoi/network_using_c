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
    int serv_sd, clnt_sd;
    FILE * fp;
    char buf[BUF_SIZE];
    int read_cnt;

    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz;

    if(argc != 2)
    {
        printf("Usage: %s <port> \n", argv[0]);
        exit(1);
    }

    /**
     * 서버의 소스파일인 file_server.c를 클라이언트에게 전송하기 위해 파일을 열고 있다
     */ 
    fp = fopen("file_server.c", "rb");
    serv_sd = socket(PF_INET, SOCK_STREAM, 0);

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sd, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");
    
    if(listen(serv_sd, 5)==-1)
        error_handling("listen() error");

    clnt_adr_sz = sizeof(clnt_adr);
    clnt_sd = accept(serv_sd, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

    /**
     * 연결된 클라이언트에게 파일 데이터를 전송하기 위한 반복문이 구성되어 있다
     */ 
    while(1)
    {
        read_cnt = fread((void*)buf, 1, BUF_SIZE, fp);

        /**
         * 읽어낸 바이트 수가 버퍼 수보다 작다는 것은
         * 파일의 끝에 도달했다는 것임
         */
        
        if(read_cnt < BUF_SIZE)
        {
            write(clnt_sd, buf, read_cnt);
            break;
        }

        write(clnt_sd, buf, BUF_SIZE);
    }

    /**
     * 파일 전송 후 출력 스트림에 대한 Half close를 진행하고 있다.
     * 클라이언트에는 EOF가 전송되고, 이를 통해서 클라이언트는 
     * 파일전송이 완료되었음을 인식할 수 있다.
     */ 
    shutdown(clnt_sd, SHUT_WR);
    read(clnt_sd, buf, BUF_SIZE);
    printf("Message from client: %s \n", buf);

    fclose(fp);
    close(clnt_sd);
    close(serv_sd);
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}