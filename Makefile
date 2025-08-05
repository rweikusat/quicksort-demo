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

.PHONY: all benchmark clean shootout

all: $(PRGS)

clean:
	rm -f $(PRGS)

benchmark: all
	scripts/benchmark bin/quicksort
	@printf '\n'
	scripts/benchmark bin/qs-faster

shootout: all
	scripts/shootout $(N)

bin/%: src/%.c
	$(CC) $(CFLAGS) -o $@ $^
