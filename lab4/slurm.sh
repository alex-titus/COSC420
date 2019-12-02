#!/bin/bash

# File: myproject.sh

#SBATCH --job-name=myproject
#SBATCH --nodes=15
#SBATCH --ntasks-per-node=10
#SBATCH --mem=4gb
#SBATCH --time=24:00:00    # Time limit in the form hh:mm:ss
#SBATCH --output=out/%j.log

module load mpi/openmpi3-x86_64

# For a python3 project
# srun python3 ~/Projects/myproject/myproject.py

# For a C MPI project

mpirun ~/COSC420/lab4/a.out
