#!/bin/bash
#SBATCH --time=23:00:00
#SBATCH -p bii
#SBATCH -n 1
#SBATCH --mem=98000
#SBATCH -A nssac_students
#SBATCH -o simu1.out
#SBATCH -e simu1.error


python extract.py
