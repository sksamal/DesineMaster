#!/bin/bash
#SBATCH --time=01:00:00          # Run time in hh:mm:ss
#SBATCH --mem-per-cpu=10000mb
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=16
#SBATCH --job-name=desine-slimnet
#SBATCH --error=/work/deogun/ssamal/DesineMaster/desine-slimnet.err
#SBATCH --output=/work/deogun/ssamal/DesineMaster/desine-slimnet.out

module load compiler/gcc/5.4
obj/desine doc/parameters-slimnet
