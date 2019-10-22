#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include "matrice.h"

int main(int argc, char **argv){
  MPI_Init(&argc, &argv);
  srand(time(0));

  // Setting up variables from command line21
  int n = atoi(argv[1]);
  int m = atoi(argv[2]);
  int k = atoi(argv[3]);
  int matrix_size = n*k;

  matrix A, B, AplusB, AminusB, BminusA, ATranspose, AtimesB;
  initMatrix(&A, n, m);
  initMatrix(&B, m, k);
  initMatrix(&AplusB, n, k);
  initMatrix(&AminusB, n, k);
  initMatrix(&BminusA, n, k);
  initMatrix(&ATranspose, m, n);
  initMatrix(&AtimesB, n, k);


  int me;
  int world_size;
  MPI_Comm world = MPI_COMM_WORLD;
  MPI_Comm_rank(world, &me);
  MPI_Comm_size(world, &world_size);

  //matrix_addition(&A, &B, &AplusB);
  //matrix_subtraction(&A, &B, &AminusB);
  //matrix_subtraction(&B, &A, &BminusA);
  //transpose(&A, &ATranspose);

  if(me == 0){
    //printf("Matrix A:\n");
    //printMatrix(&A);
    //printf("Matrix B:\n");
    //printMatrix(&B);
  }


  matrix_multiplication(&A, &B, &AtimesB);
  if (me == 0){

    printf("\nMatrix Multiplication (A x B)\n");
    printMatrix(&AtimesB);
    /*


    printf("\nMatrix Addition (A+B) and (B+A)\n");
    printMatrix(&AplusB);

    printf("\nMatrix Subtraction (A-B)\n");
    printMatrix(&AminusB);

    printf("\nMatrix Subtraction (B-A)\n");
    printMatrix(&BminusA);

    printf("\nTranspose of A is:\n");
    printMatrix(&ATranspose);
    */
  }

  MPI_Finalize();
  return 0;
}
