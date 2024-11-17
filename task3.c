#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

pid_t child_pid;

void parent_handler(int signal) {
    printf("Батьківський процес: отримав сигнал\n");
    kill(child_pid, SIGUSR1);
}

void child_handler(int signal) {
    printf("Дочірній процес: отримав сигнал\n");
    kill(getppid(), SIGUSR1);
}

int main() {
    child_pid = fork();

    if (child_pid < 0) {
        perror("Помилка створення процесу");
        return 1;
    } else if (child_pid == 0) {
        signal(SIGUSR1, child_handler);
        while (1) {
            pause();
        }
    } else {
        signal(SIGUSR1, parent_handler);
        printf("Починаємо обмін сигналами...\n");
        kill(child_pid, SIGUSR1); // Перший сигнал
        while (1) {
            pause();
        }
    }
    return 0;
}
