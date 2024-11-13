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


#define MAX_PROCESSES 3
pid_t monitored_pids[MAX_PROCESSES];
int period = 10; // Periodo di controllo in secondi

int main(int argc, char *argv[]) {
    
    sleep(1);

    FILE *logfile = fopen("logfile.txt", "r");
    if (logfile == NULL) {
        perror("Failed to open logfile");
        return 1;
    }

    int pid1, pid2, pid3;
    if (fscanf(logfile, "%d,%d,%d,", &pid1, &pid2, &pid3) != 3) {
        fprintf(stderr, "Failed to read PIDs from logfile\n");
        fclose(logfile);
        return 1;
    }
    fclose(logfile);

    monitored_pids[0] = pid1;
    monitored_pids[1] = pid2;
    monitored_pids[2] = pid3;

    // Avvio del ciclo di controllo periodico

    while (1) {
        sleep(period); // Pausa tra i controlli
        for (int i = 0; i < MAX_PROCESSES; i++) {
            if (monitored_pids[i] != 0) {
                // Invia SIGUSR1 e verifica la risposta
                //printf("monitored_pid[%d]: %d, result kill %d, SIGUSR1: %d\n", i, monitored_pids[i], kill(monitored_pids[i], SIGUSR1), SIGUSR1);
                if (kill(monitored_pids[i], SIGUSR1) == -1) {
                    printf("Process %d is not responding or has terminated\n", monitored_pids[i]);
                    monitored_pids[i] = 0; // Imposta il PID a 0 per ignorare in futuro
                } else {
                    printf("Sent SIGUSR1 to process %d\n", monitored_pids[i]);
                }
            }
        }
    }

    return 0;
}
