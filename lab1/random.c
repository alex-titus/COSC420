#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <mpi.h>

int main(int argc, char **argv){
  MPI_Init(NULL, NULL);

  int world_size, me;
  float my_number, sum, average;
  MPI_Comm world = MPI_COMM_WORLD;
  MPI_Comm_rank(world, &me);
  MPI_Comm_size(world, &world_size);

  srand(time(0) + me);
  my_number = (float) rand() / (float) RAND_MAX;
  /*
  int MPI_Allreduce(const void* sendbuf,
                    void* recvbuf,
                    int count,
                    MPI_DataType datatype,
                    MPI_Op op,
                    MPI_Comm comm)
  */
  MPI_Allreduce(&my_number, &sum, 1, MPI_FLOAT, MPI_SUM, world);
  average = sum/(float)world_size;

  printf("Rank %d's number was: %2.4f\n", me, my_number);

  if (me == 0){
    printf("The average was %2.4f\n", average);
  }

  MPI_Finalize();
  return 0;
}
