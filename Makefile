CC = gcc
CFLAGS = -Wall -g -std=c99 -Iinclude
LDFLAGS = 

SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin

LIBNAME = libopenhttp.a

CFILES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(CFILES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

all: $(BINDIR)/$(LIBNAME)

$(BINDIR)/$(LIBNAME): $(OBJECTS)
	mkdir -p $(dir $@)
	$(AR) rcs $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

install: $(BINDIR)/$(LIBNAME)
	install -m 644 $(BINDIR)/$(LIBNAME) /usr/local/lib
	mkdir -p /usr/local/include/openhttp
	rm -rf /usr/local/include/openhttp/*
	install -m 644 $(INCDIR)/*.h /usr/local/include/openhttp
	$(MAKE) install-pkgconfig

install-pkgconfig:
	install -D -m 644 openhttp.pc /usr/lib/pkgconfig/openhttp.pc

clean:
	rm -f $(OBJDIR)/*.o $(BINDIR)/$(LIBNAME)

.PHONY: all install clean
