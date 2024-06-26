# Kontig

Kontig is a complete pipeline for de-novo hybrid genome assembly. It supports both short read and long read sequencers, and a combination of the two. Kontig was originally designed for metagenomic samples, with an ability to produce gene graphs, from which hypothetical genomes can be extracted. 

## Usage

```kontig convert -i samples/pacbio-ccs/sd_0001.fastq -o test-output/input.nsf```\
Convert FASTQ files into Kontig's native NSF (Nucleotide Scratch File) format.

```kontig generate kmers -i test-output/input.nsf -o test-output/table0.klt```\
Build a sorted K-Mer table from the generated scratch file.
(NOT WORKING)

```kontig assemble --kmers test-output/table0.klt --contigs test-output/contigs0.nsf --graph test-output/connections0.cgc```\
Assemble contigs using the generated K-Mer table. Divergent connections will be logged in the graph file.
(NOT WORKING)

```kontig convert -i test-output/contigs0.nsf -o test-output/output.fa```\
Convert contigs from the assembly into a FASTA file for use with other bioinformatics tools.
(NOT WORKING)

## References

Reference genomes used for testing:\
https://www.ncbi.nlm.nih.gov/genome/1099?genome_assembly_id=300531

Read simulators used for testing:\
https://github.com/HadrienG/InSilicoSeq \
https://github.com/pfaucon/PBSIM-PacBio-Simulator

Settings used for generating simulated reads:
```
pbsim --data-type CLR --depth 20 --model_qc ../PBSIM-PacBio-Simulator/data/model_qc_clr reference-genomes/lactobacillus-reference-genome.fa
iss generate --genomes reference-genomes/lactobacillus-reference-genome.fa -n 2m --model miseq --output samples/illumina/miseq
```
