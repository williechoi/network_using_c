#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 100
void error_handling(char* message);
void read_childproc(int sig);

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    int fds[2];

    pid_t pid;
    struct sigaction act;
    socklen_t adr_sz;
    int str_len, state;
    char buf[BUF_SIZE];

    if(argc!=2)
    {
        printf("usage : %s <port> \n", argv[0]);
        exit(1);
    }

    /**
     * 좀비 프로세스를 막기 위한 코드 구성
     */ 
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;
    state = sigaction(SIGCHLD, &act, 0);

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
        error_handling("bind() error!");

    if(listen(serv_sock, 5)==-1)
        error_handling("listen() error!");

    /**
     * 파이프 생성
     */ 
    pipe(fds);

    /**
     * 파일의 데이터 저장을 담당할 프로세스 생성
     */ 
    pid = fork();

    if(pid == 0){
        FILE *fp = fopen("echomsg.txt", "wt");
        char msgbuf[BUF_SIZE];
        int i, len;

        for(i=0; i<10; i++)
        {
            len = read(fds[0], msgbuf, BUF_SIZE);
            fwrite((void*) msgbuf, 1, len, fp);
        }
        fclose(fp);
        return 0;
    }

    
    while(1)
    {
        adr_sz = sizeof(clnt_adr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
        if(clnt_sock==-1)
            continue;
        else
            puts("new client connected.");

        /**
         * accept 함수 호출 이후 fork를 실행하여 새로운 프로세스 생성
         * 부모와 자식 프로세스는 모두 소켓의 파일 디스크립터를 갖게 된다.
         */ 
        pid = fork();
        if(pid == -1){
            close(clnt_sock);
            continue;
        }

        /**
         * 각각의 프로세스에서 필요 없는 소켓은 닫아준다.
         */ 

        if(pid == 0){
            /**
             * 자식 프로세스가 실행하는 영역이다. (클라이언트에게 에코 서비스 제공)
             * 서버 소켓의 파일 디스크립터는 자식 프로세스가 알 필요가 없으므로 소멸시킨다.
             */ 
            close(serv_sock);
            while((str_len = read(clnt_sock, buf, BUF_SIZE)) != 0){
                write(clnt_sock, buf, str_len);
                write(fds[1], buf, str_len);
            }
            close(clnt_sock);
            puts("client disconnected...");
            return 0;
        } else {
            /**
             * 새로운 프로세스가 클라이언트에게 서비스를 제공하므로,
             * 메인 프로세스에서는 클라이언트와 연결된 소켓을 소멸시킨다.
             */ 
            close(clnt_sock);
        }
    }
    close(serv_sock);
    return 0;
}

void read_childproc(int sig) {
    pid_t pid;
    int status;
    pid = waitpid(-1, &status, WNOHANG);
    printf("removed proc id : %d \n", pid);
}

void error_handling(char * message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}