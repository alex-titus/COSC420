Both programs are compiled with a simple "make" command, and cleaned with "make clean"

Execution of dot is as follows:
  mpiexec -n A ./dot B
    A = number of processors you wish to run on
    B = length of each vector

Execution of vectors is as follows:
  mpiexec -n A ./dot B C D
    A = number of processors you wish to run on
    B, C, and D are used to create the matrices (B x C) and (C x D)


(a) What is the theoretical time complexity of your algorithms (best and worst case),
in terms of the input size?
  Addition + Subtraction:
    O(N/P)
      N = number of elements in the array
      P = number of processors
    The best case is when N = P, anymore and you have waste
    Worst case is N/2 when N = 2p - 1

  Multiplication:
  O(N^3/P) is both the best and worst case scenario


(b) According to the data, does adding more nodes perfectly divide the time taken by the program?
  
(c) What are some real-world software examples that would need the above routines?
  Anything with a large amount of matrice multiplcations
    Robotics, Banking, Rocket Science
Why?
  The more nodes allowed for the larger matrice greatly increases speed
Would they benefit greatly from using your distributed code?

(d) How could the code be improved in terms of usability, efficiency, and robustness?
  I believe distribution of elements is already at a best case scenario for how
  much I understand about MPI. I can see Matrix Multiplication being improved
  as I know the method I used was not optimal. Also when testing on incredibly large
  matrix  (1000 x 1000) * (1000 x 1000) my console would crash.
