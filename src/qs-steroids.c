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
#include <unistd.h>

/** constants */
enum {
    N_THREADS =		4,
    MT_THRESH =		512     /* post work to thread for larger subfields */
};

/**  types */
struct work_item {
    struct work_item *p;

    int *nums;
    unsigned l, r;
};

/**  variables */
static struct {
    struct work_item *head, **chain;
    pthread_cond_t cond;
} q = {
    .chain = &q.head,
    .cond = PTHREAD_COND_INITIALIZER
};

static struct {
    unsigned n;
    pthread_cond_t cond;
} active = {
    .cond = PTHREAD_COND_INITIALIZER
};

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static int verbose;

/**  prototypes */
static void qs(int *, unsigned, unsigned);

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

    rc = pthread_mutex_lock(&lock);
    assert(!rc);

    *q.chain = wi;
    q.chain = &wi->p;

    rc = pthread_mutex_unlock(&lock);
    assert(!rc);
    rc = pthread_cond_signal(&q.cond);
    assert(!rc);
}

static struct work_item *get_work_item(void)
{
    struct work_item *wi;
    int rc;

    rc = pthread_mutex_lock(&lock);
    assert(!rc);

    while (wi = q.head, !wi) {
        rc = pthread_cond_wait(&q.cond, &lock);
        assert(!rc);
    }

    q.head = wi->p;
    if (!q.head) q.chain = &q.head;
    ++active.n;

    rc = pthread_mutex_unlock(&lock);
    assert(!rc);

    return wi;
}

static void work_done(void)
{
    unsigned n;
    int rc;

    rc = pthread_mutex_lock(&lock);
    assert(!rc);

    n = --active.n;

    rc = pthread_mutex_unlock(&lock);
    assert(!rc);
    if (n) return;

    rc = pthread_cond_signal(&active.cond);
    assert(!rc);
}

static void *thread_run(void *arg)
{
    struct work_item *wi;
    unsigned tid;

    tid = (unsigned)arg;

    while (1) {
        wi = get_work_item();
        if (verbose) fprintf(stderr, "#%u: %u - %u\n", tid, wi->l, wi->r);
        qs(wi->nums, wi->l, wi->r);
        work_done();

        free(wi);
    }

    return NULL;
}

static void start_threads(unsigned n)
{
    pthread_t tid;
    int rc;

    while (n) {
        rc = pthread_create(&tid, NULL, thread_run, (void *)n);
        assert(!rc);

        --n;
    }
}

static void wait_for_done(void)
{
    int rc;

    rc = pthread_mutex_lock(&lock);
    assert(!rc);

    while (q.head || active.n) {
        rc = pthread_cond_wait(&active.cond, &lock);
        assert(!rc);
    }
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
        if (r - ol > MT_THRESH) post_work_item(nums, ol, r - 1);
        else qs(nums, ol, r - 1);
    }

    if (l == r) ++l;
    if (l < or) qs(nums, l, or);
}

/**  main */
int main(int argc, char **argv)
{
    int *nums, c;
    unsigned n_threads;

    n_threads = 0;
    while (c = getopt(argc, argv, "n:v"), c != -1)
        switch (c) {
        case 'n':
            n_threads = atoi(optarg);
            break;

        case 'v':
            verbose = 1;
            break;

        case '?':
            fputs("unkown option\n", stderr);
            exit(1);
        }

    argc -= optind;
    if (!argc) {
        fputs("Usage: qs-steroids [-n <# of threads>] [-v] <number>+\n", stderr);
        exit(1);
    }
    argv += optind;
    nums = malloc(argc * sizeof(int));
    fill_nums(argv, nums);

    if (n_threads < 1) n_threads = N_THREADS;
    start_threads(n_threads);

    qs(nums, 0, argc - 1);
    wait_for_done();

    if (verbose) print_nums("sorted", nums, argc);

    return 0;
}
