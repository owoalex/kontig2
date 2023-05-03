#!/bin/bash

# NOTE:
# This script assumes you have built PBSIM-Pacbio-Simulator (https://github.com/pfaucon/PBSIM-PacBio-Simulator) alongside kontig
# It expects to be in a folder at the same level *not* as a submodule

#mkdir -p samples/pacbio/clr
#cd samples/pacbio/clr
#pbsim --data-type CLR --depth 20 --model_qc ../PBSIM-PacBio-Simulator/data/model_qc_clr reference-genomes/lactobacillus-reference-genome.fa
#cd ../../..
mkdir -p samples/pacbio/ccs
cd samples/pacbio/ccs
../../../../PBSIM-PacBio-Simulator/src/pbsim --data-type CCS --depth 20 --model_qc ../../../../PBSIM-PacBio-Simulator/data/model_qc_ccs ../../../reference-genomes/lactobacillus-reference-genome.fa
cd ../../..

#iss generate --genomes reference-genomes/lactobacillus-reference-genome.fa -n 2m --model miseq --output samples/illumina/miseq
