#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int signal) {
    switch (signal) {
        case SIGINT:
            printf("Отримано сигнал SIGINT\n");
        break;
        case SIGTERM:
            printf("Отримано сигнал SIGTERM\n");
        break;
        case SIGUSR1:
            printf("Отримано сигнал SIGUSR1, завершення програми\n");
        exit(0);
        default:
            break;
    }
}

void setup_signal_handling_signal() {
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGUSR1, signal_handler);
    signal(SIGPROF, SIG_DFL);
    signal(SIGHUP, SIG_IGN);
}

void setup_signal_handling_sigaction() {
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGUSR1, &sa, NULL);

    sa.sa_handler = SIG_DFL;
    sigaction(SIGPROF, &sa, NULL);

    sa.sa_handler = SIG_IGN;
    sigaction(SIGHUP, &sa, NULL);
}

int main() {
    //setup_signal_handling_signal();
    setup_signal_handling_sigaction();

    printf("Очікування сигналів...\n");
    while (1) {
        pause();
    }
    return 0;
}
