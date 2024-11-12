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


FILE *logfile;

// Handler per SIGUSR1
void sig_handler(int signo) {
    if (signo == SIGUSR1) {
        fprintf(logfile, "Received SIGUSR1 by process: %d\n", getpid());
        fflush(logfile);
    }
}

int main() {

    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE); // Non-blocking getch

    // Apri il file di log
    logfile = fopen("/home/mattia/Desktop/arp/ARP-Watchdog/logfile.txt", "a");
    if (logfile == NULL) {
        perror("fopen");
        return 1;
    }

    // Stampa messaggio iniziale
    printw("Hello, I'm the process with PID %d.\n", getpid());
    refresh();

    // Imposta il signal handler per SIGUSR1
    signal(SIGUSR1, sig_handler);
    // Ciclo principale
    while (1) {
        pause(); // Attende i segnali in modo efficiente
    }

    endwin(); // End ncurses mode
    fclose(logfile);
    return 0;
}
