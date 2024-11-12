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

#define PROCESSNUM 3

int main() {

    pid_t pids[PROCESSNUM];             // process ID

    // riempimento pids[PROCESSNUM]
    for (int i = 0; i < PROCESSNUM; i++){
        pids[i] = fork();

        //creation error
        if (pids[i] < 0) {
            perror("Errore nella fork");
            exit(1);
        } 
        // va a buon fine
        else if (pids[i] == 0) { // Processo figlio
            char *args[] = {"konsole", "-e","./p", NULL }; // Array di argomenti per execvp

            args[0] = "konsole";
            if (execvp(args[0], args) == -1) {
                perror("Errore in execvp per w1");
                exit(1);
            }
        }
    }
    
    pid_t pidwd = fork();

    if (pidwd < 0) {
        perror("Errore nella fork");
        exit(1);
    } 
    else if (pidwd == 0) { 
    
        
        char *args[PROCESSNUM + 2];
        args[0] = "./watchdog";
        for (int i = 0; i < PROCESSNUM; i++) {
            args[i + 1] = malloc(10);
            snprintf(args[i + 1], 10, "%d", pids[i]);
        }
        args[PROCESSNUM + 1] = NULL;
        // Eseguiamo blackBoard
        if (execvp(args[0], args) == -1) {
            perror("Errore in execvp per watchdog");
            exit(1);
        }
    }

    // Attende tutti i processi figli
    for (int i = 0; i < PROCESSNUM + 1; i++) { // PROCESSNUM + 1 per includere blackBoard
        wait(NULL);
    }

    return 0;
}
