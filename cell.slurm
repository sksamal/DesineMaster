#!/bin/bash
#SBATCH --time=00:90:00          # Run time in hh:mm:ss
#SBATCH --mem-per-cpu=3500mb
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=16
#SBATCH --job-name=desine-cell
#SBATCH --error=/work/deogun/ssamal/DesineMaster/desine-cell.err
#SBATCH --output=/work/deogun/ssamal/DesineMaster/desine-cell.out

#module load compiler/gcc/4.9
obj/desine doc/parameters-cell
