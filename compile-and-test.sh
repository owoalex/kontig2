#!/bin/bash
if make; then
    echo ""
    echo "#####"
    echo ""
    rm -rf test-output
    mkdir -p test-output
    echo ">build/kontig convert -i samples/pacbio/ccs/sd_0001.fastq -o test-output/input.nsf"
    if build/kontig convert -i samples/pacbio/ccs/sd_0001.fastq -o test-output/input.nsf; then
        echo "SUCCESS"
    else
        echo ""
        echo "FAILED TEST"
        exit
    fi
    echo ">build/kontig convert -i test-output/input.nsf -o test-output/input.fa"
    if build/kontig convert -i test-output/input.nsf -o test-output/input.fa; then
        echo "SUCCESS"
    else
        echo ""
        echo "FAILED TEST"
        exit
    fi
    echo ">build/kontig generate kmers -i test-output/input.nsf"
    if build/kontig generate kmers -i test-output/input.nsf; then
        echo "SUCCESS"
    else
        echo ""
        echo "FAILED TEST"
        exit
    fi
    echo ">build/kontig assemble --kmers test-output/table0.klt --reads test-output/input.nsf --contigs test-output/contigs0.nsf --graph test-output/connections0.cgc"
    if build/kontig assemble --kmers test-output/table0.klt --reads test-output/input.nsf --graph test-output/connections0.cgc; then
        echo "SUCCESS"
    else
        echo ""
        echo "FAILED TEST"
        exit
    fi
    echo ">build/kontig join -i test-output/input.nsf -o test-output/contigs0.nsf --connections test-output/connections0.cgc --graph test-output/graph0.cgc"
    if build/kontig join -i test-output/input.nsf -o test-output/contigs0.nsf --connections test-output/connections0.cgc --graph test-output/graph0.cgc; then
        echo "SUCCESS"
    else
        echo ""
        echo "FAILED TEST"
        exit
    fi
    echo ">build/kontig convert -i test-output/contigs0.nsf -o test-output/output.fa"
    if build/kontig convert -i test-output/contigs0.nsf -o test-output/output.fa; then
        echo "SUCCESS"
    else
        echo ""
        echo "FAILED TEST"
        exit
    fi
    echo ""
    echo "#####"
    echo ""
    tail test-output/output.fa
    echo ""
    echo "#####"
    echo ""
    echo "Check assembled output at test-output/output.fa for errors"
else
    echo "BUILD FAILED"
fi
