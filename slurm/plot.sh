#!/bin/bash
#
#SBATCH -d singleton
#SBATCH --partition=defq    # Partition to submit to 
#
#SBATCH --ntasks=1
#SBATCH --time=10:00         # Maximum runtime in D-HH:MM
#SBATCH --mem-per-cpu=100    # Memory in MB per cpu allocated


venv/bin/python3 python/plot.py $Input $Output
