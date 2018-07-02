override CFLAGS += -pthread -Wno-attributes
CC = gcc

SOURCES := $(wildcard *.c)
BINARIES := $(SOURCES:%.c=%)

all: $(BINARIES)

libkdump.o: libkdump/libkdump.c
	$(CC) -c libkdump/libkdump.c -o libkdump.o $(CFLAGS)

%: %.c libkdump.o libkdump/libkdump.h
	$(CC) libkdump.o libkdump/libkdump.h $< -o $@ -static $(CFLAGS) 

clean:
	rm -f *.o $(BINARIES)