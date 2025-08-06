/*
  quicksort demo

  Second algorithm more in line with the one described by Knuth/
  Sedgewick. Somewhat faster in the sense that it usually needs less
  comparisons and swaps for the same input. The partitioning loop
  should also be somewhat faster due to simpler code but I haven't
  looked at the machine code in detail.
*/

/**  includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    ol = l;
    or = r;
    pivot = nums[l];
    do {
        ++l;
        ++comps;
        if (nums[l] > pivot) {
            while (++comps, nums[r] > pivot) --r;

            if (r > l) {
                swap(nums, l, r);
                --r;
            }
        }
    } while (l < r);

    if (r > ol) {
        swap(nums, ol, r);
        qs(nums, ol, r - 1);
    }

    if (l < or) qs(nums, l, or);
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

    qs(nums, 0, argc - 1);
    print_nums("sorted", nums, argc);
    printf("comparisons %u swaps %u\n", comps, swaps);

    return 0;
}
