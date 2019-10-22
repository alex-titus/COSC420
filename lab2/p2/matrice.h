
#define INDEX(n,m,i,j) m*i + j
#define ACCESS(A,i,j) A->arr[INDEX(A->rows, A->cols, i, j)]

typedef struct MATRIX{
  int rows, cols;
  int* arr;
} matrix;

void initMatrix(matrix*, int, int);
void printMatrix(matrix*);
void transpose(matrix*, matrix*);
int dot_product(int*, int*, int);
void matrix_addition(matrix*, matrix*, matrix*);
void matrix_subtraction(matrix*, matrix*, matrix*);
void matrix_multiplication(matrix*, matrix*, matrix*);

void printMatrix(matrix* A){
  int i, j;
  for(i = 0; i < A->rows; i++){
    for(j = 0; j < A->cols; j++){
      printf("%d ", ACCESS(A,i,j));
    }
    puts("");
  }
}

void initMatrix(matrix* A, int r, int c){
  A->rows = r;
  A->cols = c;
  A->arr = calloc(r*c, sizeof(int)); // Dynamically allocate memory
  int i,j;
  for(i = 0; i < r; i++)
    for(j = 0; j < c; j++)
      ACCESS(A,i,j) = rand() % 9 + 1;
      // Compiler will double replace with both ACCESS and INDEX
}

void transpose(matrix* A, matrix* B){
  int i, j;
  for(i = 0; i < A->rows; i++){
    for(j = 0; j < A->cols; j++){
      ACCESS(B,j,i) = ACCESS(A,i,j);
    }
  }
}

int dot_product(int* A, int *B, int C){
  int array_size = C;
  int me, i;
  int sum = 0;
  int world_size;
  MPI_Comm world = MPI_COMM_WORLD;
  MPI_Comm_rank(world, &me);
  MPI_Comm_size(world, &world_size);

  int *sendcounts = malloc(world_size * sizeof(int));
  int *displs = malloc(world_size * sizeof(int));

  int work_load = array_size / world_size;
  int extra_numbers = (array_size % world_size) + work_load;

  int rem = array_size % world_size;
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
  int *partial_array_one = malloc(extra_numbers * sizeof(int));
  int *partial_array_two = malloc(extra_numbers * sizeof(int));

  MPI_Scatterv(
   A,
   sendcounts,
   displs,
   MPI_INT,
   partial_array_one,
   sendcounts[me],
   MPI_INT,
   0,
   world
  );
  MPI_Scatterv(
   B,
   sendcounts,
   displs,
   MPI_INT,
   partial_array_two,
   sendcounts[me],
   MPI_INT,
   0,
   world
  );
  unsigned long long partial_innter_product = 0;
  for( i = 0; i < sendcounts[me]; i++){
   partial_innter_product += partial_array_one[i] * partial_array_two[i];
  }

  unsigned long full_inner_product = 0;
  MPI_Reduce(
   &partial_innter_product,
   &full_inner_product,
   1,
   MPI_UNSIGNED_LONG,
   MPI_SUM,
   0,
   world
  );
  return full_inner_product;
}

void matrix_addition(matrix* A, matrix* B, matrix* C){
  int me;
  int sum = 0;
  int world_size;
  MPI_Comm world = MPI_COMM_WORLD;
  MPI_Comm_rank(world, &me);
  MPI_Comm_size(world, &world_size);
  int matrix_size = A->rows * B->cols;

  int *sendcounts = malloc(world_size * sizeof(int));
  //int *displs = malloc(world_size * sizeof(int));
  int displs[world_size];

  int work_load = matrix_size / world_size;
  int extra_numbers = (matrix_size % world_size) + work_load;

  int rem = matrix_size % world_size;

  int i;
  // calculate send counts and displacements
   for (i = 0; i < world_size; i++) {
       sendcounts[i] = matrix_size/world_size;
       if (rem > 0) {
           sendcounts[i]++;
           rem--;
       }
       displs[i] = sum;
       sum += sendcounts[i];
   }

   int *partial_matrix_one = malloc(sendcounts[me] * sizeof(int));
   int *partial_matrix_two = malloc(sendcounts[me] * sizeof(int));

   MPI_Scatterv(
     A->arr,
     sendcounts,
     displs,
     MPI_INT,
     partial_matrix_one,
     sendcounts[me],
     MPI_INT,
     0,
     world
   );
   MPI_Scatterv(
     B->arr,
     sendcounts,
     displs,
     MPI_INT,
     partial_matrix_two,
     sendcounts[me],
     MPI_INT,
     0,
     world
   );

   for(i = 0; i < sendcounts[me]; i++){
     //printf("Addition %d\n", partial_matrix_one[i]);
     partial_matrix_one[i] += partial_matrix_two[i];
   }

   MPI_Gatherv(
     partial_matrix_one,
     sendcounts[me],
     MPI_INT,
     C->arr,
     sendcounts,
     displs,
     MPI_INT,
     0,
     world
   );
   MPI_Barrier(world);
}

void matrix_subtraction(matrix* A, matrix* B, matrix* C){
    int me;
    int sum = 0;
    int world_size;
    MPI_Comm world = MPI_COMM_WORLD;
    MPI_Comm_rank(world, &me);
    MPI_Comm_size(world, &world_size);
    int matrix_size = A->rows * B->cols;

    int *sendcounts = malloc(world_size * sizeof(int));
    int displs[world_size];

    int work_load = matrix_size / world_size;
    int extra_numbers = (matrix_size % world_size) + work_load;

    int rem = matrix_size % world_size;

    int i;
    // calculate send counts and displacements
     for (i = 0; i < world_size; i++) {
         sendcounts[i] = matrix_size/world_size;
         if (rem > 0) {
             sendcounts[i]++;
             rem--;
         }
         displs[i] = sum;
         sum += sendcounts[i];
     }

     int *partial_matrix_one = malloc(sendcounts[me] * sizeof(int));
     int *partial_matrix_two = malloc(sendcounts[me] * sizeof(int));

     MPI_Scatterv(
       A->arr,
       sendcounts,
       displs,
       MPI_INT,
       partial_matrix_one,
       sendcounts[me],
       MPI_INT,
       0,
       world
     );
     MPI_Scatterv(
       B->arr,
       sendcounts,
       displs,
       MPI_INT,
       partial_matrix_two,
       sendcounts[me],
       MPI_INT,
       0,
       world
     );

     for(i = 0; i < sendcounts[me]; i++){
       partial_matrix_one[i] -= partial_matrix_two[i];
     }

     MPI_Gatherv(
       partial_matrix_one,
       sendcounts[me],
       MPI_INT,
       C->arr,
       sendcounts,
       displs,
       MPI_INT,
       0,
       world
     );
     MPI_Barrier(world);
}

void matrix_multiplication(matrix* A, matrix* B, matrix* C){
  matrix D;
  //initMatrix(&D, B->cols, B->rows);
  //transpose(B, &D);
  int i, n, m;

  for(m = 0; m < C->rows; m++){
    for(n = 0; n < C->cols; n++){
      int array_size = A->cols;
      int *array_one = malloc(array_size * sizeof(int));
      int *array_two = malloc(array_size * sizeof(int));
      for(i = 0; i < A->cols; i++){
        array_one[i] = A->arr[(m * A->cols)+i];
        array_two[i] = B->arr[(i * B->cols)+n];
      }
      int q = dot_product(array_one, array_two, array_size);
      ACCESS(C,m,n) = q;
    }
  }
}
