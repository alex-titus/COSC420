#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include "newmatrice.h"

int main(int argc, char **argv){
  MPI_Init(&argc, &argv);
  srand(time(0));

  // Setting up variables from command line
  int n = atoi(argv[1]);
  int m = atoi(argv[2]);
  int k = atoi(argv[3]);
  int matrix_size = n*k;

  matrix A, B, AtimesB;
  initMatrix(&A, n, m);
  initMatrix(&B, m, k);
  //matrix_multiplication(&A, &B, &AtimesB);

  int me;
  int world_size;
  MPI_Comm_rank(world, &me);
  MPI_Comm_size(world, &world_size);
  if (me == 0){
    printMatrix(&A);
    printMatrix(&B);
  }

  gauss_jordan(&A, &B);

  if (me == 0){
    printf("task completed\n");
    printMatrix(&A);
    printMatrix(&B);
  }


  MPI_Finalize();
  return 0;
}
