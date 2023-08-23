#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void new_proc(int p[2]) {
    int prime;
    int n;
    int er = read(p[0],&prime,4);
    close(p[1]);
    if(er != 4) {
        fprintf(2,"read Erro(%d)\n",er);
        exit(1);
    }
    printf("prime %d\n", prime);
    if(read(p[0],&n,4)) {
        int newp[2];
        pipe(newp);
        if(fork() > 0) { //father
            close(newp[0]);
            write(newp[1],&n,4);
            while(read(p[0],&n,4)) {
                if(n%prime != 0) {
                    if(write(newp[1],&n,4) != 4) {
                        fprintf(2,"write %d Erro\n", n);
                        exit(1);
                    }
                }
            }
            close(p[0]);
            close(newp[1]);
            wait(0);
            exit(0);
        } else { // child
            new_proc(newp);
        }
    }
}


int main()
{
    int p[2];
    pipe(p);
    if(fork() > 0) { //父进程
        close(p[0]);
        for(int i = 2; i <= 35; i++) {
            if(write(p[1], &i, 4) != 4) {
                fprintf(2,"write %d Erro\n", i);
                exit(1);
            }
        }
        close(p[1]);
        wait(0);
        exit(0);
    } else { // 子进程
        new_proc(p);
    }

    exit(0);
}