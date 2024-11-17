#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

char *message;

void alarm_handler(int signal) {
    if (signal == SIGALRM) {
        printf("%s\n", message);
        exit(0);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Використання: %s <час у секундах> <повідомлення>\n", argv[0]);
        return 1;
    }

    int time = atoi(argv[1]);
    message = argv[2];

    pid_t pid = fork();
    if (pid < 0) {
        perror("Помилка створення процесу");
        return 1;
    } else if (pid == 0) {
        signal(SIGALRM, alarm_handler); // Встановлення обробника сигналу
        alarm(time);
        pause();
    } else {
        printf("Батьківський процес завершено.\n");
    }
    return 0;
}
