#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 100
void error_handling(char * buf);

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    struct timeval timeout;
    fd_set reads, cpy_reads;

    socklen_t adr_sz;
    int fd_max, str_len, fd_num, i;
    char buf[BUF_SIZE];

    if (argc != 2){
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
        error_handling("bind() error!");
    
    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    FD_ZERO(&reads);
    /**
     * fd_set 변수 reads에 서버 소켓을 등록하였음. 
     * 즉, 데이터 수신 여부를 관찰하는 관찰 대상에 서버 소켓이 포함됨
     */ 
    FD_SET(serv_sock, &reads);
    fd_max = serv_sock;
    printf("fd_max: %d \n", fd_max);

    while(1)
    {
        cpy_reads = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;

        /**
         * 관찰의 목적에 맞게 필요한 인자만 전달하면 됨
         * 즉, read (입력이 있는 소켓 파일디스크립터)만 관찰
         */ 
        if((fd_num=select(fd_max+1, &cpy_reads, 0, 0, &timeout))==-1)
            break;
        
        if(fd_num==0)
            continue;

        /**
         * select 함수가 1이상을 반환했을 때 실행되는 반복문
         */ 
        for(i=0; i<fd_max+1; i++)
        {
            /**
             * 상태 변화가 있었던 소켓 디스크립터를 찾는 문장
             */ 
            if(FD_ISSET(i, &cpy_reads)){
                /**
                 * 서버 소켓에 변화가 있었다면?
                 * 연결 요청에 대한 수락의 과정을 진행한다.
                 */ 
                if(i==serv_sock) {
                    adr_sz = sizeof(clnt_adr);
                    clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_adr, &adr_sz);
                    /**
                     * fd_set 변수 reads에 클라이언트와 연결된 소켓의
                     * 파일 디스크립터 정보를 등록한다.
                     */ 
                    FD_SET(clnt_sock, &reads);

                    /**
                     * 기존의 파일디스크립터 번호와 클라이언트 파일 디스크립터 번호를 비교하여
                     * 클라이언트 파일 디스크립터 번호가 더 크면 더 큰 값을 fd_max에 집어넣는다.
                     */ 
                    if(fd_max < clnt_sock) {
                        printf("new fd_max (clnt_sock): %d \n", clnt_sock);
                        fd_max = clnt_sock;
                    }
                    
                    printf("connected client: %d \n", clnt_sock);
                } else {
                    /**
                     * 상태변화가 일어난 소켓이 서버 소켓이 아닌 경우 실행되는 부분이다.
                     * 즉, 수신된 데이터가 있는 경우 실행하는 부분이다.
                     */ 
                    str_len = read(i, buf, BUF_SIZE);

                    /**
                     * eof가 수신된 경우
                     */ 
                    if(str_len == 0) {

                        FD_CLR(i, &reads);
                        close(i);
                        printf("closed client: %d \n", i);
                    } else {
                        /**
                         * 그렇지 않은 경우
                         */
                        write(i, buf, str_len);
                    }
                }
            }   
        }
    }
    close(serv_sock);
    return 0;
}

void error_handling(char *buf)
{
    fputs(buf, stderr);
    fputc('\n', stderr);
    exit(1);
}