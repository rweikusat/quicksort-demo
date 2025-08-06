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
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**  types */
struct work_item {
    struct work_item *p;

    int *nums;
    unsigned l, r;
};

/**  variables */
struct {
    struct work_item *head, **chain;
    pthread_mutex_t lock;
    pthread_cond_t cond;
} q = {
    .chain = &q.head,
    .lock = PTHREAD_MUTEX_INITIALIZER,
    .cond = PTHREAD_COND_INITIALIZER
};

struct {
    unsigned count;
    pthread_mutex_t lock;
    pthread_cond_t cond;
} active = {
    .lock = PTHREAD_MUTEX_INITIALIZER,
    .cond = PTHREAD_COND_INITIALIZER
};

/**  routines */
/***  MT support */
static void post_work_item(int *nums, unsigned l, unsigned r)
{
    struct work_item *wi;
    int rc;

    wi = malloc(sizeof(*wi));
    assert(wi);
    wi->p = NULL;
    wi->nums = nums;
    wi->l = l;
    wi->r = r;

    rc = pthread_mutex_lock(&q.lock);
    assert(!rc);

    *q.chain = wi;
    q.chain = &wi->p;

    rc = pthread_mutex_unlock(&q.lock);
    assert(!rc);
    rc = pthread_cond_signal(&q.cond);
    assert(!rc);
}

static struct work_item *get_work_item(void)
{
    struct work_item *wi;
    int rc;

    rc = pthread_mutex_lock(&q.lock);
    assert(!rc);

    while (wi = q.head, !wi) {
        rc = pthread_cond_wait(&q.cond, &q.lock);
        assert(!rc);
    }

    q.head = wi->p;
    if (!q.head) q.chain = &q.head;

    rc = pthread_mutex_unlock(&q.lock);
    assert(!rc);

    return wi;
}

/***  quicksort proper */
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
