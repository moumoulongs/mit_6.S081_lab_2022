#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int p[2];
    if(pipe(p) == -1) fprintf(1,"pipe(p) Erro\n");
    if(fork() > 0) { // 父进程
        char ping[1];
        ping[0] = 'i';
        write(p[1],ping,1);
        close(p[1]);
        wait((int *) 0);
        read(p[0],ping,1);
        close(p[0]);
        if(ping[0] == 'i') fprintf(1,"%d: received pong\n",getpid());
        else fprintf(2,"%d: received pong Erro(%c)\n",getpid(), ping[0]);
    } else {  // 子进程
        char pong[1];
        read(p[0],pong,1);
        write(p[1],pong,1);
        if(pong[0] == 'i') fprintf(1,"%d: received ping\n",getpid());
        else fprintf(2,"%d: received ping Erro(%c)\n",getpid(), pong[0]);
    }
    exit(0);
}