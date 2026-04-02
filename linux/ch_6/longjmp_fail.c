#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>

static jmp_buf env;

void set() {
    setjmp(env);
}

int main(int argc, char *argv[])
{
    set();
    printf("Some work...");
    longjmp(env, 1);
    return EXIT_SUCCESS;
}
