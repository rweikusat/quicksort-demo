/*
  quicksort demo
*/

/**  includes */
#include <stdio.h>
#include <stdlib.h>

/**  variables */
static unsigned comps, swaps;

/**  routines */
static void fill_nums(char **args, int *nums)
{
    char *cur;

    while (cur = *args, cur) {
        *nums++ = atoi(cur);
        ++args;
    }
}

static void print_nums(char *what, int *nums, unsigned n)
{
    printf("%s: %d", what, *nums);
    while (--n) printf(" %d", *++nums);
    putchar('\n');
}

static void swap(int *nums, unsigned p0, unsigned p1)
{
    int x;

    ++swaps;

    x = nums[p0];
    nums[p0] = nums[p1];
    nums[p1] = x;
}

static void qs(int *nums, unsigned l, unsigned r)
{
    int pivot;
    unsigned ol, or;

    switch (r + 1 - l) {
    case 2:
        if (nums[l] > nums[r]) swap(nums, l, r);

    case 1:
    case 0:
        return;
    }

    pivot = nums[r];
    ol = l;
    or = r;
    do {
        ++comps;

        if (nums[l] > pivot) {
            while (++comps, nums[r] > pivot && --r > l);

            if (r > l) {
                swap(nums, l, r);
                ++l;
            }
        } else
            ++l;
    } while (l < r);

    qs(nums, ol, l - 1);
    qs(nums, l, or);
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
    nums = malloc(argc * sizeof(int));
    fill_nums(argv + 1, nums);

//    print_nums("input", nums, argc);
    qs(nums, 0, argc - 1);
//    print_nums("sorted", nums, argc);
    printf("comparisons %u swaps %u\n", comps, swaps);

    return 0;
}
