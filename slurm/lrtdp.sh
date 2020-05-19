#!/bin/bash
#
#SBATCH --partition=longq    # Partition to submit to 
#
#SBATCH --ntasks=1
#SBATCH --time=10:00         # Maximum runtime in D-HH:MM
#SBATCH --mem-per-cpu=100    # Memory in MB per cpu allocated

./testsolver.out --$Domain=$Instance --n=$N --algorithm=$Algorithm --v=1 --pslip=0.2 --perror=0.1 --trials=$NTrials
