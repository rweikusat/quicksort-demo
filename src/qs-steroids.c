/*
  quicksort demo

  "Quicksort [...] Such a strategy is inappropriate for
  parallell computations."
  [Knuth, The Art of Computer Programmming, vol 3 p 113]

  Whether or not this make much sense would be a different
  question but it certainly can be done.

  :-)
*/

/**  includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
        if (nums[l] > pivot) {
            while (nums[r] > pivot) --r;

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

    if (l == r) ++l;
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

    return 0;
}
