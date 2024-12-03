# Test Makefile for the OpenHTTP test.

CC = gcc
CFLAGS = -Wall -g -std=c99 $(shell pkg-config --cflags openhttp)
LDFLAGS = $(shell pkg-config --libs openhttp)

test: main.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f test