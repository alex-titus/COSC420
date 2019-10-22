#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

int main(int argc, char **argv){
  MPI_Init(&argc, &argv);

  // Initializing values for
  int me;
  int world_size;
  MPI_Comm world = MPI_COMM_WORLD;
  MPI_Comm_rank(world, &me);
  MPI_Comm_size(world, &world_size);
  unsigned long int input_number = atoi(argv[1]);
  unsigned long int num = sqrt(input_number)+1;

  unsigned long int beginning_number;
  unsigned long int final_number;
  // Base case for node 0, we don't need to test below 2
  if(me == 0){
    beginning_number = 2;
    final_number = (num/world_size);
  } else if (me == world_size){
    // Base case for node n, as we don't need to test above sqrt(final_number)
    beginning_number = ((num*me)/world_size);
    final_number = num;
  } else {
    // Standard case for node 1 through node n-1
    beginning_number = ((me*num)/world_size);
    final_number = ((me+1)*num)/world_size;
  }

  // Easiest way to check if given a small number to test, so you won't test 1
  if(beginning_number == 1){
    beginning_number = 2;
  }
  if(final_number == 1){
    final_number = 2;
  }
  if(beginning_number == 0){
    beginning_number = 2;
  }
  if(final_number == 0){
    final_number = 2;
  }

  printf("Node %d is testing %lu-%lu\n", me, beginning_number, final_number);

  // For loop each node will run that will iterate through it's numbers
  unsigned long int i;
  for(i = beginning_number; i <= final_number; i++){
    if (input_number % i == 0){
      printf("Node %d found %lu is divisible by %lu\n", me, input_number, i);
    }
  }
  MPI_Finalize();
  return 0;
}
