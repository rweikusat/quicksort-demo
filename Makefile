#
# Makefile for quicksort demo
#

CC ?=	gcc
CFLAGS ?= -W -Wall -g

ifdef FINAL
CFLAGS := $(CFLAGS) -O2
endif

quicksort: quicksort.c
	$(CC) $(CFLAGS) -o $@ $^
