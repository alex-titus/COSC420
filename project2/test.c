#include <stdlib.h> // rand
#include <stdio.h> // puts, printf, etc
#include <time.h> // time
#include <mpi.h> // mpi
#include <math.h>
#include "matlib.h" //parallel matrix library

int main(int argc, char** argv){

	MPI_Init(NULL, NULL);

    Matrix* adjacency = malloc(sizeof(struct Matrix));
    mat_init(adjacency, 10, 10, 0);
    ACCESS(adjacency, 3, 8) = 1;
    ACCESS(adjacency, 2, 7) = 1;
    ACCESS(adjacency, 4, 1) = 1;
    ACCESS(adjacency, 9, 4) = 1;
    ACCESS(adjacency, 5, 2) = 1;
    ACCESS(adjacency, 5, 6) = 1;
    ACCESS(adjacency, 5, 7) = 1;
    ACCESS(adjacency, 6, 7) = 1;
    ACCESS(adjacency, 8, 7) = 1;
    ACCESS(adjacency, 9, 7) = 1;
    ACCESS(adjacency, 1, 7) = 1;
    ACCESS(adjacency, 3, 2) = 1;
    ACCESS(adjacency, 1, 2) = 1;
    ACCESS(adjacency, 3, 4) = 1;
    ACCESS(adjacency, 3, 5) = 1;
    ACCESS(adjacency, 3, 6) = 1;
    ACCESS(adjacency, 3, 7) = 1;

    pagerank(adjacency, 10, 0.85);

	MPI_Finalize();
	return 0;
}
