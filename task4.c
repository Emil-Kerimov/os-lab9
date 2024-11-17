#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

pid_t child_pid; // ідентифікатор дочірнього процесу

void child_handler(int signal, siginfo_t *info, void *context) {
    printf("Дочірній процес: отримав сигнал %d, додаткова інформація: %d\n",
           signal, info->si_value.sival_int);
}

int main() {
    child_pid = fork();

    if (child_pid < 0) {
        perror("Помилка створення процесу");
        return 1;
    } else if (child_pid == 0) { // Дочірній процес
        struct sigaction sa;  //встановлює обробник, який підтримує передачу додаткової інформації
        sa.sa_sigaction = child_handler;
        sa.sa_flags = SA_SIGINFO; //активує розширену інформацію про сигнал
        sigemptyset(&sa.sa_mask); // очищає маску сигналів
        sigaction(SIGRTMIN, &sa, NULL);

        while (1) {
            pause(); //до отримання сигналу
        }
    } else {
        for (int i = 1; i <= 5; i++) {
            sleep(1);
            union sigval value;
            value.sival_int = i;
            sigqueue(child_pid, SIGRTMIN, value); //Надсилання сигналу SIGRTMIN дочірньому процесу
        }
        sleep(1);
        kill(child_pid, SIGTERM); //Після завершення циклу надсилає сигнал SIGTERM дочірньому процесу для його завершення
        wait(NULL); //Очікує завершення дочірнього процесу
        printf("Дочірній процес завершено.\n");
    }
    return 0;
}
