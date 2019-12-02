/*
	Jack Stoetzel & Lauren Golain
	COSC 420 - 001
	Dr. Anderson
	November 1, 2019

	Lab04
*/

#define _GNU_SOURCE
#define _XOPEN_SOURCE

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <crypt.h>
#include <string.h>
#include <fcntl.h>
#include <mpi.h>

int main(int argc, char* argv[])

{
	MPI_Init(NULL, NULL);

	//----------------------------------------------------------------
	int world;

	MPI_Comm comm = MPI_COMM_WORLD;

	MPI_Comm_size(comm, &world);

	int rank;

	MPI_Comm_rank(comm, &rank);
	//----------------------------------------------------------------

	int mid;

	if(world < 10 && rank == 0){
		puts("ERROR: This program will take a LONG LONG time to do.");
		puts("Please run on AT LEAST 10 processes.");
		puts("Thank You :)");
		MPI_Finalize();
		return 0;
	}
	// If world is even, use 2 nodes to calculate words with no numbers
	if(world % 2 == 0){
		mid = 2;
	}
	else{
		mid = 1;
	}

	//**********************************************************************************

	int i, k;
  char **{''}
	int infile = open("xaa", O_RDONLY);
	char buf;
	char word[64];
	char temp[64];
	//char salt[] = "$5$ab$";
	//char* shadows[] = {"$5$ab$X6moXMBmKxEkH6XrCqHvFXTB.5WeyWWTVFqahh.BFRD", "$5$ab$xhzLWNRyZfrtjyG6HBQ48t3KCK2j4qsqDRQMNGVn6A9", "$5$ab$Uy4WKL6KBnQ.xNa5b7ZCbpKX2DPVRsKUa.ssXclDUfA", "$5$ab$ay/R9ygZUsjXmksQvlpMzbdtYBjOUvlEVCj9IaM.cnB", "$5$ab$S0uPEMdp1nHs/MmB1XVeM1jqyEnhs4z9BZe.08jFlA7", "$5$ab$rHFkI2ust2fsZNNEkYk2ImkYbTmovoMvdjlA9ozPdX4", "$5$ab$DhS5P8WyMpT9FTovr8UrllCCGAOJHAVH0549zC9TVv.", "$5$ab$Te3DSDggFW3Ds0/Hq9SxxBQITs/opgC8.xuAl1fzxo9", "$5$ab$QmqcXXxcIt2UTSMrmw9cgz7eHpE4249XesQyoQePW79", "$5$ab$is9fvJL/y3aTsrTiNWu0biOZpa2xoFqfsstktXGM3V3", "$5$ab$eLAaVmRnryQpC5JcvxLyJxM7oXYcODXUa4KM10dw/s9"};
	char salt[] = "$1$ab$";
	char* names[] = {"george", "maria", "alice", "bob", "despickler", "richard", "rmshifler", "yxjing", "jtanderson", "bfpierce", "rmstallman"};
	char* shadows[] = {"$1$ab$tYl84YhDM6bmCOuCusTKS.", "$1$ab$DC1ifnjzzu8Za5qEt96Kq0", "$1$ab$DqPMoPPboZ2HTH7RfiqQs.", "$1$ab$0I4CGceZU1wOu9PO3qn2p/", "$1$ab$FPyWVGc2x83IsQ7.q775k1", "$1$ab$T/Cabrtf2TgOYXhZFlRct/", "$1$ab$/rOLL5LFn/3ZIa2TFnN4G.", "$1$ab$N9t.xxEuc93HGa.twsZuP.", "$1$ab$Po1AuQSRCorWXHi8cIOhK/", "$1$ab$s2uqC2lMivvj9IX5PQ8KO/", "$1$ab$tThrUGNaCZBr224TYwxw2."};

	//**********************************************************************************

	char** words;
  int numberOfWords = 250000;
	words = (char**) malloc(numberOfWords * sizeof(char*));
	for(i = 0; i < numberOfWords; i++){
		words[i] = (char*) malloc(64 * sizeof(char));
	}
	for(i = 0; i < numberOfWords; i++){
		memset(word, 0, sizeof(word));
		int n = 0;
		read(infile, &buf, 1);
		while(buf != '\n'){
			//printf("%c", buf);
			word[n] = buf;
			read(infile, &buf, 1);
			n++;
		}
		//printf("%s \n", word);
		strcpy(words[i],word);
	}
	if(rank == 0)
	    puts("Dictionary has been read in. \n");

	//**********************************************************************************

	int listSize = numberOfWords;
	int divide = (world - mid) / 2;
	int block = listSize / divide;
	int overflow = listSize % divide + block;
	int sendCounts[world];
	int displs[world];
	/*
	if(rank == 0){
	printf("For %d nodes, the block size is %d with an overlfow of %d \n", world - mid, block, overflow);
}
*/
	for(i = 0; i < world - mid; i++){
		if(1 % 2 == 0)
		displs[i] = i * (block);
		else
		displs[i] = i / 2 * block;
		sendCounts[i] = block;
		if(i >= world - mid - 2){
			sendCounts[i] = overflow;
		}
	}
	if(mid == 1){
		displs[world - 1] = 0;
		sendCounts[world - 1] = numberOfWords;
	}
	if(mid == 2){
		displs[world - 2] = 0;
		displs[world - 1] = numberOfWords / 2;
		sendCounts[world - 2] = numberOfWords / 2;
		sendCounts[world - 1] = numberOfWords / 2;
	}

	/*
	if(rank == 0)
	for(i = 0; i < world - mid; i++){
	printf("displs[%d] = %d \n", i, displs[i]);
	printf("sendCounts[%d] = %d \n", i, sendCounts[i]);
	}
	*/

	//**********************************************************************************

	// One loop where every node starts and ends at different points
	for(i = displs[rank]; i < (displs[rank] + sendCounts[rank]); i++){
		// Even nodes compute the suffix numbers on every word	ex. password123
		if(rank % 2 == 0 && rank < (world - mid)){
			long long catNum;
			char num[10];
			for(catNum = 0; catNum < 1000; catNum++){
				memset(num, 0, sizeof(num));

				memset(temp, 0, sizeof(temp));
				sprintf(temp, "%s", words[i]);
				sprintf(num, "%ld", catNum);
				strcat(temp, num);
				char *pass = crypt(temp, salt);      // Encrypts a word from the file
				for(k = 0; k < 11; k++){            // Checks that word to every password in the shadow file
					if(strcmp(shadows[k], pass) == 0){
						printf("%s \t %s == %s \n", names[k], shadows[k], temp);
					}
				}
			}
		}
		// Odd nodes compute the prefix numbers on every words	ex. 99puppy
		if(rank % 2 == 1 && rank < (world - mid)){
			long long catNum;
			char num[64];
			for(catNum = 0; catNum < 1000; catNum++){
				memset(num, 0, sizeof(num));
				memset(temp, 0, sizeof(temp));
				sprintf(temp, "%s", words[i]);
				sprintf(num, "%ld", catNum);
				strcat(num, temp);
				strcpy(temp, num);
				char *pass = crypt(temp, salt);      // Encrypts a word from the file
				for(k = 0; k < 11; k++){            // Checks that word to every password in the shadow file
					if(strcmp(shadows[k], pass) == 0){
						printf("%s \t %s == %s \n", names[k], shadows[k], temp);
					}
				}
			}
		}
		// The last nodes compute words without number attached to them
		if(rank >= world - mid){
			char *pass = crypt(words[i], salt);      // Encrypts a word from the file
			for(k = 0; k < 11; k++){            // Checks that word to every password in the shadow file
				if(strcmp(shadows[k], pass) == 0){
					printf("%s \t %s == %s \n", names[k], shadows[k], words[i]);
				}
			}
		}
	}

	//****************************************************************************

	MPI_Finalize();
	close(infile);
	return 0;
}
