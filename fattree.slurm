#!/bin/bash
#SBATCH --time=03:00:00          # Run time in hh:mm:ss
#SBATCH --mem-per-cpu=3500mb
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --job-name=desine-fattree
#SBATCH --error=/work/deogun/ssamal/DesineMaster/desine-fattree.err
#SBATCH --output=/work/deogun/ssamal/DesineMaster/desine-fattree.out

module load compiler/gcc/5.4
obj/desine doc/parameters-fattree
