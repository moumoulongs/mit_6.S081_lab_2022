#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

#define DEBUG 0
#define debug(codes) if(DEBUG) {codes}

int xargs_exec(char *name, char **p_file);

int xargs(int size, char *arg[MAXARG], char *name) {
    char buf[1024];
    int m = 0, index = size;
    char *val[MAXARG];
    while(read(0,buf+m,1) == 1) {
        if(buf[m] == '\n') {
            buf[m] = 0;
            debug(
                printf("buf = %s\n", buf);
            )
            memmove(val,arg,sizeof(*arg)*size);
            val[index] = malloc(sizeof(char)*(m+1));
            memmove(val[index],buf,m+1);
            debug(
                printf("size = %d argv = %s\n", index, val[1]);
            )
            val[index+1] = 0;
            debug(
                printf("size = %d argv = %s\n", size, val[1]);
            )
            xargs_exec(name,val);
            m = 0;
        } else {
            m += 1;
        }
    }
    return 0;
}

int xargs_exec(char *name, char **p_file) {
    if(fork() == 0) {
        debug(
            printf("name = %s\n", name);
            for(int i = 0; p_file[i] != 0; i++) {
                printf("argc[%d] = %s\n", i, p_file[i]);
            }
        )
        if(exec(name,p_file) == -1) {
            fprintf(2,"exec(%s) erro\n", name);
        }
        exit(0);
    } else {
        wait(0);
    }
    return 0;
}


int main(int argc, char *argv[]) {
    debug(
        printf("%d argv:%s\n", argc,argv[1]);
    )
    xargs( argc-1, argv+1, argv[1]);
    exit(0);
}

 