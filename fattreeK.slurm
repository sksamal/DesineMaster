#!/bin/bash
#SBATCH --time=22:00:00          # Run time in hh:mm:ss
#SBATCH --mem-per-cpu=3500mb
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --job-name=desine-fattree10K
#SBATCH --error=/work/deogun/ssamal/DesineMaster/desine-fattree10K.err
#SBATCH --output=/work/deogun/ssamal/DesineMaster/desine-fattree10K.out

module load compiler/gcc/5.4
obj/desine doc/parameters-fattree10K16K
obj/desine doc/parameters-fattree10K32K
obj/desine doc/parameters-fattree10K64K
obj/desine doc/parameters-fattree10K128K
