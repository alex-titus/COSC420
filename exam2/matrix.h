
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

void number3(matrix* A, matrix* B){
  int i, j;
  srand(time(NULL));
  for(i = 0; i < 1000; i++){
    int randStart = rand() % 8;
    for(j = 0; j < 10;){
      int randA = rand() % 8;
      if(ACCESS(A,randStart,randA) == 1){
        randStart = randA;
        j++;
      }
    }
    ACCESS(B,randStart,0) = (ACCESS(B, randStart, 0))+1;
  }
  printMatrix(B);
  normalize(B, 1);
  printf("Nomralized:\n");
  printMatrix(B);
}

void fixMatrix(matrix* A){
  ACCESS(A,0,1) = 1;
  ACCESS(A,0,3) = 1;
  ACCESS(A,0,6) = 1;
  ACCESS(A,0,7) = 1;
  ACCESS(A,1,0) = 1;
  ACCESS(A,1,2) = 1;
  ACCESS(A,1,4) = 1;
  ACCESS(A,1,5) = 1;
  ACCESS(A,2,0) = 1;
  ACCESS(A,2,1) = 1;
  ACCESS(A,2,2) = 1;
  ACCESS(A,2,7) = 1;
  ACCESS(A,3,0) = 1;
  ACCESS(A,3,2) = 1;
  ACCESS(A,3,3) = 1;
  ACCESS(A,3,4) = 1;
  ACCESS(A,3,7) = 1;
  ACCESS(A,4,1) = 1;
  ACCESS(A,4,2) = 1;
  ACCESS(A,4,3) = 1;
  ACCESS(A,4,6) = 1;
  ACCESS(A,5,5) = 1;
  ACCESS(A,5,6) = 1;
  ACCESS(A,6,0) = 1;
  ACCESS(A,6,1) = 1;
  ACCESS(A,6,3) = 1;
  ACCESS(A,6,4) = 1;
  ACCESS(A,6,6) = 1;
  ACCESS(A,6,7) = 1;
  ACCESS(A,7,0) = 1;
  ACCESS(A,7,2) = 1;
  ACCESS(A,7,3) = 1;
  ACCESS(A,7,4) = 1;
  ACCESS(A,7,6) = 1;
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
  printMatrix(B);
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
