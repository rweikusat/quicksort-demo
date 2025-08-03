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

static void swap(int *nums, unsigned p0, unsigned p1)
{
    int x;

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
        if (nums[l] > nums[r - 1]) swap(nums, l, r);

    case 1:
    case 0:
        return;
    }

    pivot = nums[r];
    ol = l;
    or = r;
    while (l < r) {
        if (nums[l] > pivot) {
            while (r > l && nums[r] > pivot) --r;

            if (r > l) {
                swap(nums, l, r);
                ++l;
            }
        } else ++l;
    }

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
    nums = alloca(argc * sizeof(int));
    fill_nums(argv + 1, nums);
    qs(nums, 0, argc);
    print_nums(nums, argc - 1);

    return 0;
}
