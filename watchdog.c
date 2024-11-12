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
#include <signal.h>

int period = 10;        // period after which the check is done

// define process monitored variables
#define MAX_PROCESSES 3
pid_t monitored_pids[MAX_PROCESSES];

void handler(int signal_number) {
    period--;
    printf("%d", period);
    fflush(stdout);
    if (period == 0) {
        for (int i = 0; i < MAX_PROCESSES; i++)
            printf("Checking if process still alive %d", monitored_pids[i]);
            fflush(stdout);
            /* if (check if it is running ){

            } else {

            } */
        period = 10;
    }

}

int main(int argc, char *argv[])
{
    printf( "watchdog: %d\n", getpid());
    char *pids_str = getenv("MONITORED_PIDS");

    if (pids_str != NULL) {

        char *token = strtok(pids_str, ",");

        /*
        while (token != NULL) {

            monitored_pids[num_monitored_processes++] = token;

            token = strtok(NULL, ",");

        }
        */

    }
    
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &handler;

    while(1) {
        sigaction (SIGUSR1, &sa, NULL);
        sleep(1);
    }
    
    return 0;
}