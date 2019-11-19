#Make file for primes.c
CC=mpicc
binaries=driver

all: clean clearscreen compile

compile:
	$(CC) -lm \-o driver driver.c -std=c99

clean:
	$(RM) $(binaries)

clearscreen:
	clear
