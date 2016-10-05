#!/bin/bash
#SBATCH --time=8:00:00          # Run time in hh:mm:ss
#SBATCH --mem-per-cpu=19500mb
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --job-name=desine-bcube
#SBATCH --error=/work/deogun/ssamal/DesineMaster/desine-bcube.err
#SBATCH --output=/work/deogun/ssamal/DesineMaster/desine-bcube.out

module load compiler/gcc/5.4
#obj/desine doc/parameters-bcube32Kn14k3
#obj/desine doc/parameters-bcube64Kn16k3
#obj/desine doc/parameters-bcube128Kn19k3
obj/desine doc/parameters-bcuben18k3
