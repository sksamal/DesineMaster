#!/bin/bash
#SBATCH --time=2:00:00          # Run time in hh:mm:ss
#SBATCH --mem-per-cpu=19500mb
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --job-name=desine-slimnet
#SBATCH --error=/work/deogun/ssamal/DesineMaster/desine-slimnet.err
#SBATCH --output=/work/deogun/ssamal/DesineMaster/desine-slimnet.out

module load compiler/gcc/5.4
#obj/desine doc/parameters-slimnet10K16K1
#obj/desine doc/parameters-slimnet10K32K1
#obj/desine doc/parameters-slimnet10K64K1
#obj/desine doc/parameters-slimnet10K128K1
obj/desine doc/parameters-slimnet
