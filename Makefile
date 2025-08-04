#
# Makefile for quicksort demo
#

CC ?=	gcc
CFLAGS ?= -W -Wall -g

ifdef FINAL
CFLAGS := $(CFLAGS) -O2
endif

.PHONY: all clean

all: quicksort qs-faster

clean:
	rm -f quicksort
	rm -f qs-faster

quicksort:
qs-faster:

%: %.c
	$(CC) $(CFLAGS) -o $@ $^
