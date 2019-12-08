#!/bin/bash

# File: myproject.sh

#SBATCH --job-name=thiccums
#SBATCH --nodes=12
#SBATCH --ntasks-per-node=10
#SBATCH --mem=10gb
#SBATCH --time=24:00:00    # Time limit in the form hh:mm:ss
#SBATCH --output=out/%j.log

module load mpi/openmpi3-x86_64

# For a C MPI project

mpirun ~/code/COSC420/project2/driver
