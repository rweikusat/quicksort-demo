#
# Makefile for quicksort demo
#

CC ?=	gcc
CFLAGS ?= -W -Wall -g

ifdef FINAL
CFLAGS := $(CFLAGS) -O2
endif

PRGS :=	$(addprefix bin/, quicksort qs-faster)

.PHONY: all clean

all: $(PRGS)

clean:
	rm -f $(PRGS)

bin/%: src/%.c
	$(CC) $(CFLAGS) -o $@ $^
