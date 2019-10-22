#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <crypt.h>
#define world MPI_COMM_WORLD
#define FILESIZE 235866

int main(int argc, char** argv){
  MPI_Init(&argc, &argv);

  // Setting up MPI Information
  int rank, world_size, bufsize, nints;
  MPI_Comm_rank(world, &rank); // Node can report it's node number
  MPI_Comm_size(world, &world_size); // How many nodes we will be using
  MPI_Status status; // Current status of our MPI
  MPI_Request request; // Allows for MPI requests (Isend + Irecv)
  MPI_File fh; // Will be our shared file
  char *word = malloc(18 * sizeof(char));

  // Setting up sendcount and displs for each node
  int sum = 0;
  int i, j, sendcount, displs, remainder;
  if(rank == 0){ // Root node does the sending
    remainder = (FILESIZE%world_size); // Calculate the remainder
    for(i = 0; i < world_size; i++){ // For sending to specific nodes
      sendcount = (FILESIZE/world_size); // Calculate base amount of send
      if (remainder > 0) { // If we still have overflow
        sendcount++;
        remainder--;
      }
      displs = sum;
      // MPI_Send to send our two values to each node
      MPI_Send(&sendcount, 1, MPI_INT, i, 0, world);
      MPI_Send(&displs, 1, MPI_INT, i, 123, world);
      sum += sendcount;
    }
  }
  // MPI_Recv to recieve our two values from the root node
  MPI_Recv(&sendcount, 1, MPI_INT, 0, 0, world, MPI_STATUS_IGNORE);
  MPI_Recv(&displs, 1, MPI_INT, 0, 123, world, MPI_STATUS_IGNORE);
  // Printing out our values to keep track of data
  printf("rank %d will be reading from word %d to %d\n", rank, displs, displs+sendcount);

  // Open up our file in MPI, allow for parallel I/O
  MPI_File_open(MPI_COMM_WORLD,"words",MPI_MODE_RDONLY,MPI_INFO_NULL,&fh);
  int result = MPI_File_read_at(fh, displs, word, sendcount, MPI_CHAR, MPI_STATUS_IGNORE);
  printf("Result from MPI_File_read_at: %d\n", result);
    //printf("%s", word);

  MPI_File_close(&fh);
  MPI_Finalize();
}
