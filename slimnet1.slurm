#!/bin/bash
#SBATCH --time=22:00:00          # Run time in hh:mm:ss
#SBATCH --mem-per-cpu=39500mb
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --job-name=desine-slimnet10K
#SBATCH --error=/work/deogun/ssamal/DesineMaster/desine-slimnet10K.err
#SBATCH --output=/work/deogun/ssamal/DesineMaster/desine-slimnet10K.out

module load compiler/gcc/5.4
obj/desine doc/parameters-slimnet10K16K
obj/desine doc/parameters-slimnet10K32K
#obj/desine doc/parameters-slimnet10K64K
#obj/desine doc/parameters-slimnet10K128K
