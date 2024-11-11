#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>  
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    pid_t pid = getpid();
    while (1)
    {
        printf( "watchdog: %d\n", pid);
        sleep(5);
    }
    
    return 0;
}