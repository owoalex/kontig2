#ifndef TRANSLATORS_FILE_TYPE_H
#define TRANSLATORS_FILE_TYPE_H

namespace Translators {
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
