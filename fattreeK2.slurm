#!/bin/bash
#SBATCH --time=03:00:00          # Run time in hh:mm:ss
#SBATCH --mem-per-cpu=23500mb
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --job-name=desine-fattree
#SBATCH --error=/work/deogun/ssamal/DesineMaster/desine-fattree-k2.err
#SBATCH --output=/work/deogun/ssamal/DesineMaster/desine-fattree-k2.out

module load compiler/gcc/5.4
obj/desine doc/parameters-fattree10K16K
