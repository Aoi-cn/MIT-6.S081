#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
main(int argc, char *argv[])
{
    int p_p2c[2];
    int p_c2p[2];
    char buf[10];
    pipe(p_c2p);
    pipe(p_p2c);
    if(fork() == 0)
    {
        close(p_p2c[1]);
        close(p_c2p[0]);

        read(p_p2c[0], buf, sizeof(buf));
        printf("%d: received ping\n", getpid());
        close(p_p2c[0]);

        write(p_c2p[1], "Hello!", 6);
        close(p_c2p[1]);
        
        exit(0);
    }
    else
    {
        close(p_c2p[1]);
        close(p_p2c[0]);

        write(p_p2c[1], "Hello!", 6);
        close(p_p2c[1]);

        read(p_c2p[0], buf, sizeof(buf));
        close(p_c2p[0]);
        printf("%d: received pong\n", getpid());
        
        exit(0);
    }
}