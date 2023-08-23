#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char * fmtname(char *path) {
    static char buf[DIRSIZ+1];
    char *p;
    // 找到最后一个‘/’的位置
    for(p = path+strlen(path); p>=path && *p != '/'; p--)
    ;
    p++;
    //返回以空格填充的名称
    if(strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
    return buf;
}

void find(char *path, char *aim) {
    char buf[512], *p;
    int fd;
    struct dirent de; //目录
    struct stat st; //文件信息

    if((fd = open(path, 0)) < 0) { //打开文件
        fprintf(2,"find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0) { //复制文件信息
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type){
    case T_DEVICE:
    case T_FILE:
        fprintf(2,"find: should find in a file\n");
        break;
    case T_DIR:
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
            printf("ls: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf+strlen(buf);
        *p++ = '/';
        while(read(fd, &de, sizeof(de)) == sizeof(de)) {
            if(de.inum == 0) continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if(stat(buf, &st) < 0) {
                printf("find: cannot stat %s\n", buf);
                continue;
            }
            switch (st.type){// 对目录中的文件进行查找，文件夹进行递归
            case T_DEVICE:
                if(strcmp(de.name,aim) == 0) {
                    printf("%s\n", buf);
                }
                break;
            case T_FILE:
                if(strcmp(de.name,aim) == 0) {
                    printf("%s\n", buf);
                }
                break;
            case T_DIR:
                if(strcmp(".",de.name) != 0 && strcmp("..",de.name) != 0)
                {
                    find(buf,aim);
                }
                break;
            }
        }
        break;
    }
    close(fd);

}

int main(int argc, char *argv[]) {
    if(argc < 3) {
        find(".",argv[1]);
        exit(0);
    }
    find(argv[1],argv[2]);
    exit(0);
}