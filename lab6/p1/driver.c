#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <math.h>
#include "newmatrice.h"

int main(int argc, char **argv){
  MPI_Init(&argc, &argv);

  // Setting up variables from command line
  int n = atoi(argv[1]);
  int matrix_size = n*n;
  matrix A, B, C, D;
  initMatrix(&A, n, n);
  initValuedMatrix(&B, n, 1, 1.0);
  initValuedMatrix(&C, n, 1, 1.0);

  // Setting up MPI Information
  int me;
  int world_size;
  MPI_Comm_rank(world, &me);
  MPI_Comm_size(world, &world_size);

  eigenvector(&A, &B, &C);
  MPI_Finalize();
  return 0;
}
