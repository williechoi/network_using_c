#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>


#define BUF_SIZE    4
#define EPOLL_SIZE  50

void setnonblockingmode(int fd);
void error_handling(char* buf);

int main(int argc, char* argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t adr_sz;
    int str_len, i;
    char buf[BUF_SIZE];

    struct epoll_event *ep_events;
    struct epoll_event event;
    int epfd, event_cnt;

    if(argc != 2)
    {
        printf("Usage: %s <port> \n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
        error_handling("bind() error!\n");
    if(listen(serv_sock, 5)==-1)
        error_handling("listen() error!\n");

    epfd = epoll_create(EPOLL_SIZE);
    ep_events = malloc(sizeof(struct epoll_event)*EPOLL_SIZE);

    setnonblockingmode(serv_sock);
    event.events = EPOLLIN;
    event.data.fd = serv_sock;
    epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);

    while(1)
    {
        event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
        if(event_cnt == -1)
        {
            puts("epoll_wait() error");
            break;
        }
        puts("return epoll_wait");  // 이벤트의 발생횟수를 확인하기 위한 문장

        for(i=0; i<event_cnt; i++){
            if(ep_events[i].data.fd == serv_sock) {
                adr_sz = sizeof(clnt_adr);
                clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
                setnonblockingmode(clnt_sock); // accept 함수 호출에 의한 소켓을 넌블로킹 소켓으로 변경
                event.events = EPOLLIN | EPOLLET;   // 소켓의 이벤트 등록방식을 엣지 트리거로 설정
                event.data.fd = clnt_sock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);
                printf("connected client : %d \n", clnt_sock);
            } else {
                while(1){   // 엣지 트리거 방식에서는 이벤트 발생 시 입력버퍼에 존재하는 데이터를 모두 수신해야 하므로 read를 반복해서 호출
                    str_len = read(ep_events[i].data.fd, buf, BUF_SIZE);
                    if(str_len==0) {
                        epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
                        close(ep_events[i].data.fd);
                        printf("closed client: %d \n", ep_events[i].data.fd);
                        break;
                    } else if (str_len < 0){
                        // read 함수가 -1을 반환하고, errno에 저장된 값이 EAGAIN일 경우 
                        // 입력버퍼에 저장된 데이터를 모두 읽어들였기 때문에
                        // break를 통해 반복문을 빠져나가야 함.
                        if(errno==EAGAIN) 
                            break;
                    } else {
                        write(ep_events[i].data.fd, buf, str_len);
                    }
                }
            }
        }
    }
    close(serv_sock);
    close(epfd);
    return 0;
}

void setnonblockingmode(int fd)
{
    int flag = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flag|O_NONBLOCK);
}

void error_handling(char* buf)
{
    fputs(buf, stderr);
    fputc('\n', stderr);
    exit(1);
}