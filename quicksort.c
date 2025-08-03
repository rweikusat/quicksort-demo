/*
  quicksort demo
*/

/**  includes */
#include <alloca.h>
#include <stdio.h>
#include <stdlib.h>

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
