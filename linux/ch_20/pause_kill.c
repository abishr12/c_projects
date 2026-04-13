#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/_types/_sigset_t.h>
#include <sys/signal.h>

static void handler (int sig) {
    printf("Signal received\n");
}


int main(int argc, char *argv[]) {
    sigset_t blockSet, emptySet;
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);
    sigemptyset(&emptySet);

    sa.sa_flags = 0;
    sa.sa_handler = handler;
    sigaction(SIGINT, &sa, NULL);

    sigemptyset(&blockSet);
    sigaddset(&blockSet, SIGINT);
    if(sigprocmask(SIG_BLOCK, &blockSet, NULL) == -1) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }

    raise(SIGINT);
    printf("Hello\n");

    sa.sa_handler = SIG_IGN;
    sigaction(SIGINT, &sa, NULL);

    if(sigprocmask(SIG_SETMASK, &emptySet, NULL) == -1) {
        perror("sigprocmask2");
        exit(EXIT_FAILURE);
    }

}
