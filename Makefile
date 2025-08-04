#
# Makefile for quicksort demo
#

CC ?=	gcc
CFLAGS ?= -W -Wall -g

ifdef FINAL
CFLAGS := $(CFLAGS) -O2
endif

all: quicksort qs-faster

quicksort:
qs-faster:

%: %.c
	$(CC) $(CFLAGS) -o $@ $^
