#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig)
{
    if(sig==SIGALRM){
        puts("Time out!");
    }
    alarm(2);
}

void keycontrol(int sig)
{
    int ch = 0;
    if(sig==SIGINT){
        puts("GAME PAUSED");
        puts("Press Z to resume...");
        while(1)
        {
            ch = getchar();
            if(ch == 'Z' || ch == 'z'){
                puts("Game On!");
                break;
            }
            putchar(ch);
        }
    }
}

int main(int argc, char *argv[])
{
    int i;
    struct sigaction act;
    act.sa_handler = keycontrol;
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;
    
    sigaction(SIGINT, &act, 0);

    puts("Game Start!");
    sleep(2);

    puts("Choose Your Player!");
    sleep(2);

    puts("You are Garen! Let's go to the Summoner's rift!");
    sleep(2);

    puts("Game Over");
    return 0;
}