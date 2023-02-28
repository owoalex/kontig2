# kontig2

Reference genomes used for testing:
https://www.ncbi.nlm.nih.gov/genome/1099?genome_assembly_id=300531

Read simulators used for testing:
https://github.com/HadrienG/InSilicoSeq
https://cgm.sjtu.edu.cn/PaSS/
https://github.com/pfaucon/PBSIM-PacBio-Simulator

Settings used for generating simulated reads:
pbsim --data-type CLR --depth 20 --model_qc ../PBSIM-PacBio-Simulator/data/model_qc_clr samples/lactobacillus-reference-genome.fa
iss generate --genomes samples/lactobacillus-reference-genome.fa -n 2m --model miseq --output samples/illumina/miseq
