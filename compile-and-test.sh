#!/bin/bash
if make; then
    echo ""
    echo "#####"
    echo ""
    echo ">build/kontig convert -i samples/pacbio-ccs/sd_0001.fastq -o test-output/input.nsf"
    build/kontig convert -i samples/pacbio-ccs/sd_0001.fastq -o test-output/input.nsf
    echo ">build/kontig generate kmers -i test-output/input.nsf -o test-output/table0.klt"
    build/kontig generate kmers -i test-output/input.nsf -o test-output/table0.klt
    echo ">build/kontig assemble --kmers test-output/table0.klt --contigs test-output/contigs0.nsf --graph test-output/connections0.cgc"
    build/kontig assemble --kmers test-output/table0.klt --contigs test-output/contigs0.nsf --graph test-output/connections0.cgc
    echo ">build/kontig convert -i test-output/contigs0.nsf -o test-output/output.fa"
    build/kontig convert -i test-output/contigs0.nsf -o test-output/output.fa
    #build/kontig genmap --input sample-staph.fastq --ksize 32 -o sample-staph.kmf
else
    echo "BUILD FAILED"
fi
