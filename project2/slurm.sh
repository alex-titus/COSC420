#!/bin/bash

# File: myproject.sh

#SBATCH --job-name=thiccums
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=2
#SBATCH --mem=10gb
#SBATCH --time=24:00:00    # Time limit in the form hh:mm:ss
#SBATCH --output=out/%j.log

module load mpi/openmpi3-x86_64

# For a C MPI project

mpirun ~/COSC420/project2/driver
