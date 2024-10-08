#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void
find(char *path, char *name)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }
    switch(st.type){
    case T_FILE:
        printf("find: cannot find file in a file.\n%d %d %l\n", st.type, st.ino, st.size);
        // break;
    case T_DIR:
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
            printf("find: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf+strlen(buf);
        *p++ = '/';
        while(read(fd, &de, sizeof(de)) == sizeof(de)){
            if(de.inum == 0 || !strcmp(".", de.name) || !strcmp("..", de.name))
                continue;
            if(!strcmp(de.name, name)){
                printf("%s/%s\n", path, name);
            }
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if(stat(buf, &st) < 0){
                printf("find: cannot stat %s\n", buf);
                continue;
            }
            if(st.type == T_DIR)
                find(buf, name);
        }
        break;
    }
    close(fd);
}

int
main(int argc, char *argv[])
{
    if(argc != 3){
        printf("usage: find folder name.\n");
        exit(0);
    }
    find(argv[1], argv[2]);
    exit(0);
}