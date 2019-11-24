#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <math.h>
#include "matrix.h"

int main(int argc, char **argv){
  MPI_Init(&argc, &argv);


  matrix A, B, C;
  initValuedMatrix(&A, 8, 8, 0);
  //matrix *B;
  initValuedMatrix(&B, 8, 1, 0);
  //initValuedMatrix(&B, 8, 1, 1);
  //initValuedMatrix(&C, 8, 1, 1);
  fixMatrix(&A);
  //printMatrix(&A);
  // Setting up MPI Information
  int me;
  int world_size;
  MPI_Comm_rank(world, &me);
  MPI_Comm_size(world, &world_size);
  //eigenvector(&A, &B, &C);
  number3(&A, &B);
  MPI_Finalize();
  return 0;
}
