/* Exercise 7-1: Print program break after each malloc() to demonstrate
   that malloc() allocates memory in larger chunks, not per-call via sbrk()
   
   Based on Listing 7-1: memalloc/free_and_sbrk.c */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "tlpi_hdr.h"

#define MAX_ALLOCS 1000000

int
main(int argc, char *argv[])
{
    char *ptr[MAX_ALLOCS];
    int freeStep, freeMin, freeMax, blockSize, numAllocs, j;
    void *prevBreak, *currBreak;

    printf("\n");

    if (argc < 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s num-allocs block-size [step [min [max]]]\n", argv[0]);

    numAllocs = getInt(argv[1], GN_GT_0, "num-allocs");
    if (numAllocs > MAX_ALLOCS)
        cmdLineErr("num-allocs > %d\n", MAX_ALLOCS);

    blockSize = getInt(argv[2], GN_GT_0 | GN_ANY_BASE, "block-size");

    freeStep = (argc > 3) ? getInt(argv[3], GN_GT_0, "step") : 1;
    freeMin  = (argc > 4) ? getInt(argv[4], GN_GT_0, "min")  : 1;
    freeMax  = (argc > 5) ? getInt(argv[5], GN_GT_0, "max")  : numAllocs;

    if (freeMax > numAllocs)
        cmdLineErr("free-max > num-allocs\n");

    prevBreak = sbrk(0);
    printf("Initial program break:          %10p\n", prevBreak);

    printf("Allocating %d*%d bytes\n", numAllocs, blockSize);
    for (j = 0; j < numallocs; j++) {
        ptr[j] = malloc(blocksize);
        if (ptr[j] == NULL)
            errExit("malloc");

        currBreak = sbrk(0);
        /* Only print when the program break actually changes */
        if (currBreak != prevBreak) {
            printf("malloc[%d]: program break changed: %p -> %p (+%ld bytes)\n",
                    j, prevBreak, currBreak,
                    (long)((char *)currBreak - (char *)prevBreak));
            prevBreak = currBreak;
        }
    }

    printf("Program break is now:           %10p\n", sbrk(0));

    printf("Freeing blocks from %d to %d in steps of %d\n",
            freeMin, freeMax, freeStep);

    for (j = freeMin - 1; j < freeMax; j += freeStep)
        free(ptr[j]);

    printf("After free(), program break is: %10p\n", sbrk(0));

    exit(EXIT_SUCCESS);
}
