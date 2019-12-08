#Make file for primes.c
CC=mpicc
CFLAGS= -lm -std=c99 -g -Wno-unused-result -Wno-maybe-uninitialized
binaries=driver
LDFLAGS=

all: clearscreen clean compile

compile:
	$(CC) $(CFLAGS) \-o driver driver.c

clean:
	$(RM) $(binaries)

clearscreen:
	clear
