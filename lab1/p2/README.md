Compiled with make
Executed with mpiexec -n 10 ./sqrt #
  The # is the number you want to pass in
Cleaned with make clean

(a) What is the theoretical time complexity of your algorithms (best and worst case), in terms
of the input size?
  It seems to work in roughly the same time sense for any numbers given in, the largest part of the time being the communication between nodes slowing it down immensely.
(b) According to the data, does adding more nodes perfectly divide the time taken by the program?
  It does not, it seems that a vast majority of the time (roughly .6 seconds) is the time that communication takes, and this is almost 90% of the entire time for the problem to be run
(c) Justify the fact that one needs only check up to √
N in the brute force primality test.
  Because every number after the square root is already a factor of a number between 2 and √n
(d) How could the code be improved in terms of usability, efficiency, and robustness?
  Faster communication from host machine to nodes would be the biggest time saver.
