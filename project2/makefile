#Make file for primes.c
CC=mpicc
CFLAGS= -lm -std=c99
LDFLAGS=

all: clean clearscreen compile

compile:
	$(CC) $(CFLAGS) \-o driver driver.c

clean:
	$(RM) $(binaries)

clearscreen:
	clear
