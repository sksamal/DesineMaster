#!/bin/bash
#SBATCH --time=2:00:00          # Run time in hh:mm:ss
#SBATCH --mem-per-cpu=19500mb
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --job-name=desine-bcube
#SBATCH --error=/work/deogun/ssamal/DesineMaster/desine-bcube16K.err
#SBATCH --output=/work/deogun/ssamal/DesineMaster/desine-bcube16K.out

module load compiler/gcc/5.4
obj/desine doc/parameters-bcube16Kn7k4
obj/desine doc/parameters-bcube16Kn5k5
obj/desine doc/parameters-bcube16Kn4k6
obj/desine doc/parameters-bcube16Kn3k9
obj/desine doc/parameters-bcube16Kn2k13
