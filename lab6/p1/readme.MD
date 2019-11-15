README

7. Include a README file to document your code, any interesting design choices you made, and answer the
following questions:
(a) What is the theoretical time complexity of your algorithms (best and worst case), in terms of the
input size?
The theoretical time complexity of my algorithm is actually pretty shitty, due to me being a big dumb dumb,
I believe it to be  O(N^4/P) is both the best and worst case scenario, since it's matrix multiplcation with
extra steps each time
(b) According to the data, does adding more nodes perfectly divide the time taken by the program?
It does not, there appears to be a very slight speedup, but when attempting to do a 6 node run on a 30000 by 30000
matrix my program never finished and I stopped running it after about 10 minutes.
(c) What are some real-world software examples that would need the above routines? Why? Would
they benefit greatly from using your distributed code?
Anything needing to find large matrix eigen values:
  Rocking Engineering, Banking, Cryptology, COSC420
(d) How could the code be improved in terms of usability, efficiency, and robustness?
Figure out the errors with using 6 nodes on a 30000 by 30000 matrix, and add another
check to find the eigenvalues in sequential and check my parallel version of it.

Example output!

bash-4.2$ make
mpicc -lm \-o driver driver.c
bash-4.2$ mpirun -n 3 driver 10
Eigenvalue for a 10 by 10: Value:7.032134 took 0.000000 seconds

bash-4.2$ mpirun -n 3 driver 100
Eigenvalue for a 100 by 100: Value:22.396268 took 0.000000 seconds

bash-4.2$ mpirun -n 3 driver 1000
Eigenvalue for a 1000 by 1000: Value:70.724708 took 0.070000 seconds

bash-4.2$ mpirun -n 3 driver 10000
Eigenvalue for a 10000 by 10000: Value:223.602507 took 3.490000 seconds

bash-4.2$ mpirun -n 3 driver 30000
Eigenvalue for a 30000 by 30000: Value:387.296429 took 55.250000 seconds

bash-4.2$ mpirun -n 6 driver 10
Eigenvalue for a 10 by 10: Value:7.032134 took 0.000000 seconds

bash-4.2$ mpirun -n 6 driver 100
Eigenvalue for a 100 by 100: Value:22.396268 took 0.000000 seconds

bash-4.2$ mpirun -n 6 driver 1000
Eigenvalue for a 1000 by 1000: Value:70.724708 took 0.090000 seconds

bash-4.2$ mpirun -n 6 driver 10000
Eigenvalue for a 10000 by 10000: Value:223.602507 took 3.350000 seconds


driver.c

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


newmatrice.h

#define INDEX(n,m,i,j) m*i + j
#define ACCESS(A,i,j) A->arr[INDEX(A->rows, A->cols, i, j)]
#define world MPI_COMM_WORLD

typedef struct MATRIX{
  int rows, cols;
  double* arr;
} matrix;

void initMatrix(matrix*, int, int);
void initValuedMatrix(matrix*, int, int, double);
void printMatrix(matrix*);
double dot_product(double*, double*, int);
void matrix_multiplication(matrix*, matrix*, matrix*);
double normalize(matrix*, int);
void eigenvector(matrix*, matrix*, matrix*);
void setEqual(matrix*, matrix*);

void printMatrix(matrix* A){
  int i, j;
  for(i = 0; i < A->rows; i++){
    for(j = 0; j < A->cols; j++){
      printf("%f ", ACCESS(A,i,j));
    }
    puts("");
  }
}

void initMatrix(matrix* A, int r, int c){
  A->rows = r;
  A->cols = c;
  A->arr = calloc(r*c, sizeof(double)); // Dynamically allocate memory
  int i,j;
  for(i = 0; i < r; i++)
    for(j = 0; j < c; j++)
      ACCESS(A,i,j) = rand() % 9 + 1;
      // Compiler will float replace with both ACCESS and INDEX
}

void initValuedMatrix(matrix* A, int r, int c, double value){
  A->rows = r;
  A->cols = c;
  A->arr = calloc(r*c, sizeof(double)); // Dynamically allocate memory
  int i,j;
  for(i = 0; i < r; i++)
    for(j = 0; j < c; j++)
      ACCESS(A,i,j) = value;
      // Compiler will float replace with both ACCESS and INDEX
}

double dot_product(double* A, double *B, int C){
  int array_size = C;
  int me, i;
  int sum = 0;
  int world_size;
  MPI_Comm_rank(world, &me);
  MPI_Comm_size(world, &world_size);

  int *sendcounts = malloc(world_size * sizeof(int));
  int *displs = malloc(world_size * sizeof(int));

  int work_load = array_size / world_size;
  int extra_numbers = (array_size % world_size) + work_load;

  double rem = array_size % world_size;
  // calculate send counts and displacements
  for (i = 0; i < world_size; i++) {
     sendcounts[i] = array_size/world_size;
     if (rem > 0) {
         sendcounts[i]++;
         rem--;
     }
     displs[i] = sum;
     sum += sendcounts[i];
  }
  double *partial_array_one = malloc(extra_numbers * sizeof(double));
  double *partial_array_two = malloc(extra_numbers * sizeof(double));

  MPI_Scatterv(
   A,
   sendcounts,
   displs,
   MPI_DOUBLE,
   partial_array_one,
   sendcounts[me],
   MPI_DOUBLE,
   0,
   world
  );
  MPI_Scatterv(
   B,
   sendcounts,
   displs,
   MPI_DOUBLE,
   partial_array_two,
   sendcounts[me],
   MPI_DOUBLE,
   0,
   world
  );

  double partial_innter_product = 0;
  for( i = 0; i < sendcounts[me]; i++){
   partial_innter_product += partial_array_one[i] * partial_array_two[i];
  }

  double full_inner_product = 0;
  MPI_Reduce(
   &partial_innter_product,
   &full_inner_product,
   1,
   MPI_DOUBLE,
   MPI_SUM,
   0,
   world
  );
  free(partial_array_one);
  free(partial_array_two);
  free(displs);
  free(sendcounts);
  return full_inner_product;
}

void matrix_multiplication(matrix* A, matrix* B, matrix* C){
  int i, n, m;
  for(m = 0; m < C->rows; m++){
    for(n = 0; n < C->cols; n++){
      int array_size = A->cols;
      double *array_one = malloc(array_size * sizeof(double));
      double *array_two = malloc(array_size * sizeof(double));
      for(i = 0; i < A->cols; i++){
        array_one[i] = A->arr[(m * A->cols)+i];
        array_two[i] = B->arr[(i * B->cols)+n];
      }
      double q = dot_product(array_one, array_two, array_size);
      ACCESS(C,m,n) = q;
      free(array_one);
      free(array_two);
    }
  }
}

void setEqual(matrix* A, matrix* B){
  int i, j;
  double total = 0;
  for(i = 0; i < A->rows; i++){
    for(j = 0; j < A->cols; j++){
      (ACCESS(A,i,j) = ACCESS(B,i,j));
    }
  }
}

double normalize(matrix* A, int normalize){
  int i, j, k;
  double total = 0;
  double temp = 0;
  for(i = 0; i < A->rows; i++){
    for(j = 0; j < A->cols; j++){
      temp = (double)(ACCESS(A,i,j)*ACCESS(A,i,j));
    }
    total += temp;
  }
  double newTotal = sqrt(total);
  if (normalize == 1){
    for(i = 0; i < A->rows; i++){
      for(j = 0; j < A->cols; j++){
        ACCESS(A,i,j) = ((double)ACCESS(A,i,j)/newTotal);
      }
    }
  }
  return sqrt(newTotal);
}

void eigenvector(matrix* A, matrix* B, matrix* C){
  int me;
  int world_size;
  MPI_Comm_rank(world, &me);
  MPI_Comm_size(world, &world_size);
  double temp;
  double smallestValue = 1000000000;
  clock_t begin = clock();
  /* here, do your time-consuming job */
  do {
    matrix_multiplication(A, B, C);
    if(me == 0){
      temp = smallestValue;
      setEqual(B,C);
      smallestValue = normalize(B, 1);
    }
  } while ((fabs(temp - smallestValue)) > .000001);

  matrix_multiplication(A, B, C);
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  if (me == 0){
    double normalOne = normalize(C, 0);
    double normalTwo = normalize(B, 0);
    //[[Ax]]2 = norm of C
    printf("Value:%f took %f seconds\n", (normalOne/normalTwo), time_spent);
  }
  MPI_Finalize();

}
