#Make file for primes.c
CC=gcc
CFLAGS= -lm -std=c99 -g
binaries=driver
LDFLAGS=

all: clearscreen clean compile

compile:
	$(CC) $(CFLAGS) \-o driver driver.c

clean:
	$(RM) $(binaries)

clearscreen:
	clear
