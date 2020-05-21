#!/bin/bash
#
#SBATCH --partition=defq    # Partition to submit to 
#
#SBATCH --ntasks=1
#SBATCH --time=60:00         # Maximum runtime in D-HH:MM
#SBATCH --mem-per-cpu=1000    # Memory in MB per cpu allocated

./testsolver.out --$Domain=$Instance --n=$N --algorithm=flares --v=1 --pslip=0.2 --perror=0.1 --trials=$NTrials --optimal=true --prob=0 $Additional
