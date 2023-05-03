#ifndef UTILS_FILE_TYPE_H
#define UTILS_FILE_TYPE_H

namespace Utils {
    enum FileType {
        Unknown = 0,
        GZ,
        FASTA,
        FASTQ,
        SAM,
        BAM,
        NSF,
        KLT,
        CGC
    };
}

#endif
