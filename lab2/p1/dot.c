#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

void initArray(int *A, int k){
  int i;
  for(i = 0; i < k; i++){
    A[i] = rand() % 100 +1;
  }
}

void printArray(int *A, int k){
  int i;
  for(i = 0; i < k; i++){
    printf("%d ", A[i]);
  }
  puts("");
}

int main(int argc, char **argv){
  MPI_Init(&argc, &argv);
  srand(time(0));
  int array_size = atoi(argv[1]);

  int me;
  int sum = 0;
  int world_size;
  MPI_Comm world = MPI_COMM_WORLD;
  MPI_Comm_rank(world, &me);
  MPI_Comm_size(world, &world_size);

  int *array_one = malloc(array_size * sizeof(int));
  int *array_two = malloc(array_size * sizeof(int));
  int *sendcounts = malloc(world_size * sizeof(int));
  int *displs = malloc(world_size * sizeof(int));

  int work_load = array_size / world_size;
  int extra_numbers = (array_size % world_size) + work_load;

  int rem = array_size % world_size;

  initArray(array_one, array_size);
  initArray(array_two, array_size);
  int i;
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
   // print calculated send counts and displacements for each process
   /*
   if (0 == me) {
       for (i = 0; i < world_size; i++) {
           printf("sendcounts[%d] = %d\tdispls[%d] = %d\n", i, sendcounts[i], i, displs[i]);
       }
   }
   */

   MPI_Scatterv(
     array_one,
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
     array_two,
     sendcounts,
     displs,
     MPI_INT,
     partial_array_two,
     sendcounts[me],
     MPI_INT,
     0,
     world
   );
   int partial_innter_product = 0;
   for( i = 0; i < sendcounts[me]; i++){
     partial_innter_product += partial_array_one[i] * partial_array_two[i];
   }

   int full_inner_product = 0;
   MPI_Reduce(
     &partial_innter_product,
     &full_inner_product,
     1,
     MPI_INT,
     MPI_SUM,
     0,
     world
   );
   if(me == 0){
     //printArray(array_one, array_size);
     //printArray(array_two, array_size);
     printf("Inner Product: %d\n", full_inner_product);
   }
   free(array_one);
   free(array_two);
   free(partial_array_one);
   free(partial_array_two);
   free(sendcounts);
   free(displs);

   MPI_Finalize();
   return 0;
}
