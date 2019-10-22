/*
for Pivot row k = 1, 2, . . . , n do
  Compute the vector of scalings `
  L[^k vi] = Ai,k/Ak,k ∀i = 1, . . . , n.
  // Broadcast the vector L and perform the following on n nodes:
  for Each row r != k do
    for Each column c = 1, 2, . . . m do
      Ar,c ← Ar,c − L[^k vr] · Ak,c
    end for
    for Each column c of b do
      br,c ← br,c − L[]^k vr]· bk,c
    end for
  end for
end for
*/
1 12 2 | 3
3 4 5 | 6
7 8 9 | 10

A[0,0]/A[0,0] = L[0] = 1/1
A[1,0]/A[0,0] = L[1] = 3/1
A[2,0]/A[0,0] = L[2] = 7/1

1 12 2 | 3
3-L[1]*A[0.0] 4-L[1]*A[0,1] - 5-L[1]*A[0,2] | 6-L[1]*B[1,0]
7-L[2]*A[0.0] 8-L[2]*A[0,1] - 9-L[2]*A[0,2] | 10-L[2]*B[2,0]

1 12 2 | 3
3-(3*1) 4-(3*12) 5-(3*2) | 6-(3*3)
7-(7*1) 8-(7*12) 9-(7*2) | 10-(7*3)

1 12 2 | 3
3-(3) 4-(36) 5-(6) | 6-(9)
7-(7) 8-(84) 9-(14) | 10-(21)

1  12    2 | 3
0  32  -1 | -3
0 -76  -5 | -11

2 8 1 | 2
8 6 8 | 6
2 4 7 | 5

A[0,0]/A[0,0] = L[0] = 2/2 = 1
A[1,0]/A[0,0] = L[1] = 8/2 = 4
A[2,0]/A[0,0] = L[2] = 2/2 = 1

2 8 1 | 2
8-L[1]*A[0.0] 6-L[1]*A[0,1] - 8-L[1]*A[0,2] | 6-L[1]*B[1,0]
2-L[2]*A[0.0] 4-L[2]*A[0,1] - 7-L[2]*A[0,2] | 5-L[2]*B[2,0]

2 8 1 | 2
8-(4*2) 6-(4*8) - 8-(4*1) | 6-(4*2)
2-(1*2) 4-(1*8) - 7-(1*1) | 5-(1*2)

2 8 1 | 2
8-(8) 6-(32) - 8-(4) | 6-(4*2)
2-(2) 4-(8) - 7-(1) | 5-(1*2)

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
 if (0 == me) {
     for (i = 0; i < world_size; i++) {
         printf("sendcounts[%d] = %d\tdispls[%d] = %d\n", i, sendcounts[i], i, displs[i]);
     }
 }

for(k = 0; k < maximum_pivot_rows; k++){
  for(j = 0; j < array_size; j++){
    printf("\nj:%d k:%d", j, k);
    for(i = 0; i < array_size; i++){
      L_array_A[i] = (ACCESS(A,i,k)/(double)ACCESS(A,k,k));
      partial_array_A[i] = ACCESS(A,j,i);
      //partial_array_B[i] = ACCESS(A,0,i);
    }
    if(k != j){
      printf("\nL array:\n");
      for(m = 0; m < array_size; m++){
        printf("%.3f ", L_array_A[m]);
      }
      printf("\nWorking on rows: \n");
      for(m = 0; m < array_size; m++){
        printf("%.3f ", partial_array_A[m]);
      }
      printf("test\n");
    }
  }
}


for(k = 0; k < A->rows; k++){
  for(j = 0; j < A->cols; j++){
    printf("\nj:%d k:%d", j, k);
    if(me == 0){
      L_array_A[i] = (ACCESS(A,j,k)/(float)ACCESS(A,k,k));
    }
    //BROADCAST L VECTOR TO ALL RANKS
    MPI_Bcast(L_array_A, array_size, MPI_FLOAT, 0, world);
  }
  printf("\nNode %d recieved: \n", me);
  for(m = 0; m < array_size; m++){
    printf("%.3f ", L_array_A[m]);
  }
}

for(k = 0; k < A->rows; k++){
  for(j = 0; j < array_size; j++){
    for(i = 0; i < array_size; i++){
      if (me == 0){
        L_array_A[i] = (ACCESS(A,i,k)/(double)ACCESS(A,k,k));
      }
      //partial_array_A[i] = ACCESS(A,k,i);
      //partial_array_B[i] = ACCESS(A,0,i);
    }
    MPI_Bcast(L_array_A, array_size, MPI_FLOAT, 0, world);
    MPI_Wait(&request, &status);
    if(k != j){
      printf("\nj:%d k:%d", j, k);
      printf("\nnode %d L vector:\n", me);
      for(m = 0; m < array_size; m++){
        printf("%.3f ", L_array_A[m]);
      }
      /*
      printf("\nWorking on rows: \n");
      for(m = 0; m < array_size; m++){
        printf("%.3f ", partial_array_A[m]);
      }
      */
    }
  }
}

}
