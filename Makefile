#
# Makefile for quicksort demo
#

CC ?=	gcc
CFLAGS ?= -W -Wall -g

ifdef FINAL
CFLAGS := $(CFLAGS) -O2
endif

N ?= 16

PRGS :=	$(addprefix bin/, quicksort qs-faster)

.PHONY: all clean shootout

all: $(PRGS)

clean:
	rm -f $(PRGS)

shootout: all
	scripts/shootout $(N)

bin/%: src/%.c
	$(CC) $(CFLAGS) -o $@ $^
