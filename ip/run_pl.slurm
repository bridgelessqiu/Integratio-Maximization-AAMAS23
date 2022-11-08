#!/bin/bash
#SBATCH --time=23:00:00
#SBATCH -p bii
#SBATCH -n 1
#SBATCH --mem=98000
#SBATCH -A nssac_students
#SBATCH -o simu1.out
#SBATCH -e simu1.error

module load gurobi/9.1.1

export PYTHONPATH=$GUROBI_HOME/lib/python3.6_utf32

python3 ip_solver.py pl_200_10 20
python3 ip_solver.py pl_400_10 20
python3 ip_solver.py pl_600_10 20

