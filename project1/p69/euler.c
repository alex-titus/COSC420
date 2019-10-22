/*******************************************************************************
Euler's Totient function, φ(n) [sometimes called the phi function],
is used to determine the number of numbers less than n which are
relatively prime to n. For example, as 1, 2, 4, 5, 7, and 8,
are all less than nine and relatively prime to nine, φ(9)=6.

n	  Relatively Prime	φ(n)	n/φ(n)
2	  1	                1	    2
3	  1,2	              2	    1.5
4	  1,3	              2	    2
5	  1,2,3,4	          4	    1.25
6	  1,5	              2	    3
7	  1,2,3,4,5,6	      6	    1.1666...
8	  1,3,5,7	          4	    2
9	  1,2,4,5,7,8	      6   	1.5
10	1,3,7,9	          4	    2.5

1/10 2/10 3/10 4/10 5/10 6/10 7/10 8/10 9/10
1/10 3/10 7/10 9/10
1/5  2/5  3/5  4/5
1/2

It can be seen that n=6 produces a maximum n/φ(n) for n ≤ 10.

Find the value of n ≤ 1,000,000 for which n/φ(n) is a maximum.
*******************************************************************************/

/*******************************************************************************
Current Idea:
  So we count from the number the node gets, to the number it gets
  plus the displacement it gets.

  IE (Node 3 gets 1666667 and 1666667, so does 1666667 to (1666667+1666667))
  for every testNumber (i) to our currentNumber (j 1666667-3333334) in this.

  For every non-prime number (i%j == 0),


*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#define world MPI_COMM_WORLD

int gcd(int a, int b){
  // Everything divides 0
  if (a == 0){
    return b;
  }
  if (b == 0){
    return a;
  }
  // base case
  if (a == b){
    return a;
  }
  if (a > b){
    return gcd(a-b, b);
  }
  return gcd(a, b-a);
}

int main(int argc, char** argv){
  MPI_Init(&argc, &argv);
  int n = atoi(argv[1]); // We will be taking n from the command line

  // Setting up MPI Information
  int me;
  int world_size;
  MPI_Comm_rank(world, &me);
  MPI_Comm_size(world, &world_size);
  MPI_Status status;
  MPI_Request request;

  int remainder;
  int sendcount = 0;
  int displs = 0;
  int sum = 0;
  int i, j;
  if(me == 0){
    remainder = (n%world_size);
    for(i = 0; i < world_size; i++){
      sendcount = (n/world_size);
      if (remainder > 0) {
          sendcount++;
          remainder--;
      }
      displs = sum;
      MPI_Send(&sendcount, 1, MPI_INT, i, 0, world);
      MPI_Send(&displs, 1, MPI_INT, i, 123, world);
      sum += sendcount;
    }
  }

  MPI_Recv(&sendcount, 1, MPI_INT, 0, 0, world, MPI_STATUS_IGNORE);
  MPI_Recv(&displs, 1, MPI_INT, 0, 123, world, MPI_STATUS_IGNORE);

  printf("Node %d calculating %d to %d\n",
  me, displs, displs+sendcount);

  int currentNumber, testNumber, k, l;
  double beginningNumber = displs;
  double endNumber = displs+sendcount;
  double relativelyPrime;
  //n/φ(n)
  double currentMax = 0;
  double tempMax = 0;
  //n
  int maxN = 0;

  /*
  //Count from beginningNumber to endNumber
  for(currentNumber = beginningNumber; currentNumber <= endNumber; currentNumber++){
    //Count up from 1 to our currentNumber into k
    relativelyPrime = (currentNumber-1);
    for(testNumber = 1; testNumber < currentNumber; testNumber++){
      if(gcd(currentNumber, testNumber) != 1){
        relativelyPrime--;
      }
    }
    //Finding n/φ(n)
    tempMax = testNumber/(double)relativelyPrime;
    if (tempMax >= currentMax){
      currentMax = (double)tempMax;
      maxN = testNumber;
    }
  }
  */

  int primeNumbers[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53,
    59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 
    137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199};
  size_t size = sizeof(primeNumbers)/sizeof(primeNumbers[0]);
  double phi = 1;
  // phi(n) = n*(1-1/p1)*(1-1/p2)*...*(1-1/pn)
  // n/phi(n) = 1/(1-1/p1)*(1-1/p2)*...*(1-1/pn)

  //Count from beginningNumber to endNumber
  for(currentNumber = beginningNumber; currentNumber <= endNumber; currentNumber++){
    phi = 1;
    for(j = 0; j < size; j++){
      if (currentNumber % primeNumbers[j] == 0 && currentNumber > primeNumbers[j]){
        //printf("%d mod %d = 0\n", currentNumber, primeNumbers[j]);
        phi *= (1-1/(double)primeNumbers[j]);
        //printf("1 - 1/%d = %.3f\n", phi, primeNumbers[j]);
      }
      tempMax = 1/(double)phi;
    }
    if (tempMax >= currentMax){
      currentMax = (double)tempMax;
      maxN = currentNumber;
    }
    //printf("currentNumber++\n");
  }

  printf("Node %d currentMax: %.3f and maxN: %d\n",
  me, currentMax, maxN);
  MPI_Finalize();
}
