#!/bin/bash
if make; then
    echo ""
    echo "#####"
    echo ""
    echo ">build/kontig convert -i sample-h3n2.bam -o sample-h3n2.fq"
    build/kontig convert -i sample-h3n2.bam -o sample-h3n2.fq
    #build/kontig genmap --input sample-staph.fastq --ksize 32 -o sample-staph.kmf
else
    echo "BUILD FAILED"
fi
