#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/signal.h>
#include <unistd.h>

static void defer_handler (int sig)
{
    printf("Entering Handler\n");

    sleep(5);

    printf("Exiting handler\n");
}

static void reset_handler (int sig)
{
    printf("Handler Activated\n");

}

int main(int argc, char *argv[])
{
    struct sigaction sa;
    
    sigemptyset(&sa.sa_mask);

    // sa.sa_flags = SA_RESETHAND;
    // sa.sa_handler = reset_handler;

    // sa.sa_flags = SA_NODEFER;
    sa.sa_flags = 0;
    sa.sa_handler = defer_handler;

    sigaction(SIGINT, &sa, NULL);

    for (;;)
        pause();

    return EXIT_SUCCESS;
}
