#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define INDEX(n,m,i,j) n*i + j
#define ACCESS(A,i,j) A->arr[INDEX(A->rows, A->cols, i, j)]

struct matrix{
  int rows, cols;
  int* arr;
};

void initMatrix(struct matrix* A, int r, int c){
  A->rows = r;
  A->cols = c;
  A->arr = malloc(r*c*sizeof(int));

  int i,j;
  for(i=0; i<r; i++)
    for(j=0; j<c; j++)
      ACCESS(A,i,j) = rand() % 100 + 1;
}

void printMatrix(struct matrix* A){
  int i,j;
  for(i=0; i<A->rows; i++){
    for(j=0; j<A->cols; j++){
      printf("%d ", ACCESS(A,i,j));
    }
    //printf("\n");
    puts("");
  }
}

int main(){
  srand(time(0));

  struct matrix A;
  initMatrix(&A, 3, 4);
  printMatrix(&A);
  struct matrix B;
  initMatrix(&B, 4, 3);
  printMatrix(&B);

  free(A.arr);

  //printf("Element: %d\n", );
}
