#!/bin/bash
#
#SBATCH -d singleton
#SBATCH --partition=defq    # Partition to submit to 
#
#SBATCH --ntasks=1
#SBATCH --time=10:00         # Maximum runtime in D-HH:MM
#SBATCH --mem-per-cpu=100    # Memory in MB per cpu allocated


venv/bin/python3 python/compare.py $Input $Output
curl -F file=@$Output -F "initial_comment=Hello, Leadville" -F channels=experiment -H "Authorization: Bearer $SLACK_TOKEN" https://slack.com/api/files.upload
