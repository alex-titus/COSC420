#!/bin/bash

# File: myproject.sh

#SBATCH --job-name=myproject
#SBATCH --ntasks=100
#SBATCH --mem=2gb
#SBATCH --time=00:05:00    # Time limit in the form hh:mm:ss
#SBATCH --output=out/%j.log

module load mpi/openmpi3-x86_64

# For a python3 project
# srun python3 ~/Projects/myproject/myproject.py

# For a C MPI project

sleep 15

mpirun echo test
mpirun ~/hpc/lab3/p1/driver 1000 1000 1000
