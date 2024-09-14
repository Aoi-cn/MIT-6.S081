#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    if(argc < 2)
    {
        fprintf(2, "Usage: sleep [n](seconds)\n");
        exit(1);
    }
    
    int n = atoi(argv[1]);
    int result = sleep(n);

    if(result < 0)
    {
       printf("sleep: ERROR!\n"); 
       exit(1);
    }
    exit(0);
}