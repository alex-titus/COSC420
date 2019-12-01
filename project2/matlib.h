#include <stdlib.h> // rand
#include <stdio.h> // puts, printf, etc
#include <time.h> // time
#include <mpi.h> // mpi stuff
#include <math.h> //math

#define INDEX(n,m,i,j) m*i + j
#define ACCESS(A,i,j) A->arr[INDEX(A->rows, A->cols, i, j)]

typedef struct Matrix{
	int rows, cols;
	double* arr;
} Matrix;

void mat_init(Matrix* A, int r, int c, int max_rand){
	A->rows = r;
	A->cols = c;
	A->arr = calloc(r*c,sizeof(double));

	int i,j;
	for(i=0; i<r; i++)
		for(j=0; j<c; j++)
			ACCESS(A,i,j) = rand() % max_rand + 1;
}

void mat_print(Matrix* A){
	int i,j;
	for(i=0; i<A->rows; i++){
		for(j=0; j<A->cols; j++){
			printf("%f ", ACCESS(A,i,j));
		}
		//printf("\n");
		puts("");
	}
}
void mat_transpose(Matrix* A, Matrix* B){
	if(A->rows == B->cols && A->cols == B->rows){
		int i, j;
		for(i = 0; i < A->cols; i++){
			for(j = 0; j < A->rows; j++){
				ACCESS(B,i,j) = ACCESS(A,j,i);
			}
		}
	}else
		printf("Matrix Transpose is not valid on this set of matricies.\n");
}//end mat_transpose

void mat_inplace_transpose(Matrix* A){
	int i, j;
	for(i = 0; i < A->rows; i++){
		for(j = i; j < A->cols; j++){
			double x = ACCESS(A,i,j);
			ACCESS(A,i,j) = ACCESS(A,j,i);
			ACCESS(A, j, i) = x;
		}
	}
}//end mat_transpose

void mat_add(Matrix* A, Matrix* B, Matrix* result)
{
	if( A->rows != B->rows || A->cols != B->cols)
	{
		printf("Operation not possible with those matricies");
	}
	else
	{
		MPI_Comm world = MPI_COMM_WORLD;
		int rank, world_size;
		MPI_Comm_rank(world, &rank);
		MPI_Comm_size(world, &world_size);

		int array_size = A->rows * A->cols;
		int chunk_size = array_size / (world_size);
		int leftovers = (array_size % world_size) + chunk_size;

		int send_counts[world_size];
		int displs[world_size];

		int i;

		for( i = 0; i < world_size; i++ )
		{
			displs[i] = i * (chunk_size);
			if( i == world_size - 1)
				send_counts[i] = leftovers;
			else
				send_counts[i] = chunk_size;
		}

		int recieve_array1[send_counts[rank]];
		for( i = 0; i < send_counts[rank]; i++ )
			recieve_array1[i] = 0;

		int recieve_array2[send_counts[rank]];
		for( i = 0; i < send_counts[rank]; i++ )
			recieve_array2[i] = 0;

		MPI_Scatterv(
			A->arr,
			send_counts,
			displs,
			MPI_INT,
			recieve_array1,
			send_counts[rank],
			MPI_INT,
			0,
			world
		);

		MPI_Scatterv(
			B->arr,
			send_counts,
			displs,
			MPI_INT,
			recieve_array2,
			send_counts[rank],
			MPI_INT,
			0,
			world
		);

		for( i = 0; i < send_counts[rank]; i++)
			recieve_array1[i] += recieve_array2[i];

		MPI_Gatherv(
			recieve_array1,
			send_counts[rank],
			MPI_INT,
			result->arr,
			send_counts,
			displs,
			MPI_INT,
			0,
			world
		);

	}

}//end mat_add

void mat_subtract(Matrix* A, Matrix* B, Matrix* result)
{
	if( A->rows != B->rows || A->cols != B->cols)
	{
		printf("Operation not possible with those matricies");
	}
	else
	{
		MPI_Comm world = MPI_COMM_WORLD;
		int rank, world_size;
		MPI_Comm_rank(world, &rank);
		MPI_Comm_size(world, &world_size);

		int array_size = A->rows * A->cols;
		int chunk_size = array_size / (world_size);
		int leftovers = (array_size % world_size) + chunk_size;

		int send_counts[world_size];
		int displs[world_size];

		int i;

		for( i = 0; i < world_size; i++ )
		{
			displs[i] = i * (chunk_size);
			if( i == world_size - 1)
				send_counts[i] = leftovers;
			else
				send_counts[i] = chunk_size;
		}
		int recieve_array1[send_counts[rank]];
		for( i = 0; i < send_counts[rank]; i++ )
			recieve_array1[i] = 0;

		int recieve_array2[send_counts[rank]];
		for( i = 0; i < send_counts[rank]; i++ )
			recieve_array2[i] = 0;

		MPI_Scatterv(
			A->arr,
			send_counts,
			displs,
			MPI_INT,
			recieve_array1,
			send_counts[rank],
			MPI_INT,
			0,
			world
		);

		MPI_Scatterv(
			B->arr,
			send_counts,
			displs,
			MPI_INT,
			recieve_array2,
			send_counts[rank],
			MPI_INT,
			0,
			world
		);

		for( i = 0; i < send_counts[rank]; i++)
			recieve_array1[i] -= recieve_array2[i];

		MPI_Gatherv(
			recieve_array1,
			send_counts[rank],
			MPI_INT,
			result->arr,
			send_counts,
			displs,
			MPI_INT,
			0,
			world
		);

	}

}//end mat_subtract

void mat_multiply (Matrix* A, Matrix* B, Matrix* C){

	MPI_Comm world = MPI_COMM_WORLD;
	int rank, world_size;
	MPI_Comm_rank(world, &rank);
	MPI_Comm_size(world, &world_size);

	if(A->cols == B->rows && A->rows == C->rows && B->cols == C->cols){
		int i,j,k,m;
		double row[A->cols];
		double col[A->cols];
		for(m = 0; m < C->rows; m++){
			for(j = 0; j < C->cols; j++){
				for(k = 0; k < A->cols; k++){
					row[k] = A->arr[(m*A->cols)+k];
					col[k] = B->arr[j+(k*B->cols)];
				}
				int array_size = A->cols;
				int chunk_size = array_size / (world_size);
				int leftovers = (array_size % world_size) + chunk_size;
				int send_counts[world_size];
				int displs[world_size];

				for(i = 0; i < world_size; i++){
					displs[i] = i * (chunk_size);
					if( i == world_size - 1)
						send_counts[i] = leftovers;
					else
						send_counts[i] = chunk_size;
				}
				double recieve_array1[send_counts[rank]];
				for(i = 0; i < send_counts[rank]; i++ )
					recieve_array1[i] = 0;

				double recieve_array2[send_counts[rank]];
				for(i = 0; i < send_counts[rank]; i++ )
					recieve_array2[i] = 0;

				MPI_Scatterv(
					row,
					send_counts,
					displs,
					MPI_DOUBLE,
					recieve_array1,
					send_counts[rank],
					MPI_DOUBLE,
					0,
					world
				);

				MPI_Scatterv(
					col,
					send_counts,
					displs,
					MPI_DOUBLE,
					recieve_array2,
					send_counts[rank],
					MPI_DOUBLE,
					0,
					world
				);

				double pip = 0;
				for( i = 0; i < send_counts[rank]; i++)
					pip += recieve_array1[i] * recieve_array2[i];

				double fip = 0;
				MPI_Reduce(
					&pip,
					&fip,
					1,
					MPI_DOUBLE,
					MPI_SUM,
					0,
					world
				);
				if(rank == 0)
					ACCESS(C,m,j) = fip;
			}
		}
	}else{
		if(rank == 0)
			printf("Matrix Multiplication is not valid on this set of matrices.\n");
	}
}//end mat_multiply
void mat_chunk_multiply (Matrix* A, Matrix* B, Matrix* C){
	MPI_Comm world = MPI_COMM_WORLD;
	int rank, world_size;
	MPI_Comm_rank(world, &rank);
	MPI_Comm_size(world, &world_size);

	if(A->cols == B->rows && A->rows == C->rows && B->cols == C->cols){
		mat_inplace_transpose(B);
		int i,j,k,m;

		int side_len = A->cols;
		int chunk_size = (side_len / (world_size)) * side_len;
		int leftovers = ((side_len % world_size) + chunk_size) * side_len;
		int send_counts[world_size];
		int displs[world_size];

		for(i = 0; i < world_size; i++){
			displs[i] = i * (chunk_size);
			if( i == world_size - 1)
				send_counts[i] = leftovers;
			else
				send_counts[i] = chunk_size;
		}
		double recieve_array1[send_counts[rank]];
		for(i = 0; i < send_counts[rank]; i++ )
			recieve_array1[i] = 0;

		double recieve_array2[send_counts[rank]];
		for(i = 0; i < send_counts[rank]; i++ )
			recieve_array2[i] = 0;

		MPI_Scatterv(
			A,
			send_counts,
			displs,
			MPI_DOUBLE,
			recieve_array1,
			send_counts[rank],
			MPI_DOUBLE,
			0,
			world
		);

		MPI_Scatterv(
			B,
			send_counts,
			displs,
			MPI_DOUBLE,
			recieve_array2,
			send_counts[rank],
			MPI_DOUBLE,
			0,
			world
		);

		double pip = 0;
		for( i = 0; i < send_counts[rank]; i++)
			pip += recieve_array1[i] * recieve_array2[i];

		double fip = 0;
		MPI_Reduce(
			&pip,
			&fip,
			1,
			MPI_DOUBLE,
			MPI_SUM,
			0,
			world
		);
		if(rank == 0)
			ACCESS(C,m,j) = fip;
	}
}//end mat_chunk_multiply

void mat_equals(Matrix* A, Matrix* B){
	if(A->rows == B->rows && A->cols == B->cols){
		int i, j;
		for(i = 0; i < A->cols; i++){
			for(j = 0; j < A->rows; j++)
				ACCESS(A,i,j) = ACCESS(B,i,j);
		}
	}else
		printf("Matrix equals is not valid on this set of matricies.\n");
}//end mat_equals

double norm2(Matrix* A){
	double sum = 0;
	int i;
	for(i = 0; i < A->rows; i++){
		sum += ACCESS(A, i, 0) * ACCESS(A, i, 0);
	}
	return sqrt(sum);
}//end norm2

double normalize(Matrix* A){
	double norm;
	norm = norm2(A);
	int i;
	for(i = 0; i < A->rows; i++){ //this is for vectors
		ACCESS(A, i, 0) = ACCESS(A, i, 0)/norm;
	}
	return norm;
}//end normalize

int t_check(Matrix* A, Matrix* B, double tolerance){
	int i, bool = 1;
	for(i = 0; i < A->rows; i++)
		if(fabs(ACCESS(A, i, 0) - ACCESS(B, i, 0)) < tolerance)
			bool = 0;
	return bool;
}
