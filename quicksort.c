/*
  quicksort demo
*/

/**  includes */
#include <alloca.h>
#include <stdio.h>
#include <stdlib.h>

/**  routines */
static void fill_nums(char **args, int *nums)
{
    char *cur;

    while (cur = *args, cur) {
        *nums++ = atoi(cur);
        ++args;
    }
}

static void print_nums(int *nums, unsigned n)
{
    printf("%d", *nums);
    while (--n) printf(" %d", *++nums);
    putchar('\n');
}

/**  main */
int main(int argc, char **argv)
{
    int *nums;

    if (argc < 2) {
        fputs("Usage: quicksort <number>+\n", stderr);
        exit(1);
    }

    --argc;
    nums = alloca(argc * sizeof(int));
    fill_nums(argv + 1, nums);
    print_nums(nums, argc);

    return 0;
}
