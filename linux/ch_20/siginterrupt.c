#include <signal.h>
#include <sys/signal.h>
#include <stdio.h>

int siginterrupt(int sig, int flag)
{
    struct sigaction old_sa, new_sa;

    if (sigaction(sig, NULL, &old_sa) == -1) {
        return -1;
    }
    new_sa.sa_handler = old_sa.sa_handler;
    new_sa.sa_mask = old_sa.sa_mask;
    new_sa.sa_flags = old_sa.sa_flags;
        
    if (flag == 0) {
        new_sa.sa_flags = old_sa.sa_flags | SA_RESTART;
    } 

    if (flag == 1) {
        new_sa.sa_flags = old_sa.sa_flags & ~SA_RESTART;
    }

    if (sigaction(sig, &new_sa, &old_sa) == -1) {
        return -1;
    }

    return 0;
}

// Cleaner way of wrtiting

int siginterrupt(int sig, int flag)
{
    struct sigaction sa;
    if (sigaction(sig, NULL, &sa) == -1)
        return -1;
    
    if (flag == 0)
        sa.sa_flags |= SA_RESTART;
    else
        sa.sa_flags &= ~SA_RESTART;
    
    if (sigaction(sig, &sa, NULL) == -1)
        return -1;
    
    return 0;
}
