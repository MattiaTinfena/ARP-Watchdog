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
    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE); // Non-blocking getch

    // Reset the log file at the beginning
    FILE *logfile = fopen("/home/mattia/Desktop/arp/ARP-Watchdog/logfile.txt", "w");
    if (logfile == NULL) {
        perror("fopen");
        endwin(); // End ncurses mode
        return 1;
    }
    fclose(logfile);

    pid_t pid = getpid();
    logfile = fopen("/home/mattia/Desktop/arp/ARP-Watchdog/logfile.txt", "a");
    if (logfile == NULL) {
        perror("fopen");
        return 1;
    }

    int random_fd = open("/dev/urandom", O_RDONLY);
    if (random_fd == -1) {
        perror("open");
        fclose(logfile);
        return 1;
    }

    unsigned int random_value;
    if (read(random_fd, &random_value, sizeof(random_value)) != sizeof(random_value)) {
        perror("read");
        close(random_fd);
        fclose(logfile);
        return 1;
    }

    int sleep_time = random_value % 10; // Random sleep time between 0 and 9 seconds
    
    printw("Hello, I'm the watchdog process with PID %d. Press 'q' to pause\n", pid);
    
    while (1) {
        // Non-blocking getch
        time_t now = time(NULL);
        char *time_str = ctime(&now);
        time_str[strlen(time_str) - 1] = '\0'; // Remove the newline character

        fprintf(logfile, "PID: %d, Time: %s, Delay: %d\n", pid, time_str, sleep_time);
        fflush(logfile);
        timeout(0); // Non-blocking getch
        int ch = getch();
        if (ch == 'q') {
            while (1) {
                // Restore blocking getch
                printw("Press s to resume\n");
                int ch = getch();
                if (ch == 's') {
                    break;
                }
                usleep(100000); // Sleep for 100ms to avoid busy waiting
            }
        }
        sleep(sleep_time);
    }

    endwin(); // End ncurses mode
    close(random_fd);
    fclose(logfile);
    return 0;
}