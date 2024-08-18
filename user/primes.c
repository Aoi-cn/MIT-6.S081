#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


void
getPrime(int p[2])
{
    int result = 0;
    close(p[1]);
    read(p[0], &result, sizeof(int));
    if(result != 0)
        printf("prime %d\n", result);
    else
    {
        close(p[0]);
        exit(0);
    }
    
    int p2[2];
    pipe(p2);
    if(fork() == 0)
    {
        close(p2[0]);
        int tmp = 0;
        while(read(p[0], &tmp, sizeof(int)))
        {
            if(tmp % result != 0)
                write(p2[1], &tmp, sizeof(int));
        }
        close(p2[1]);
        exit(0);
    }
    else
    {
        getPrime(p2);
        exit(0);
    }
}

int
main(int argc, char *argv[])
{
    int p[2];
    pipe(p);
    if(fork() == 0)
    {
        close(p[0]);
        for(int i = 2; i <= 35; i++)
        {
            write(p[1], &i, sizeof(i));
        }
        close(p[1]);
    }
    else
    {
        getPrime(p);
    }
    exit(0);
}