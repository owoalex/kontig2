#include <stdint.h>
#include <stdlib.h>
#include <cstring>
#include <stdio.h>
#include "TypeDetector.h"

char* Utils::TypeDetector::extract_file_extension(char* filename) {
    int index = 0;
    while (filename[index] != 0) {
        index++;
    }
    while (index > 0) {
        index--;
        if (filename[index] == '.') {
            return filename + index;
        }
    }
    return NULL;
}

Utils::FileType Utils::TypeDetector::file_extension_to_filetype(char* ext) {
    if (ext == NULL) {
        return FileType::Unknown;
    }
    if (ext[0] == '.') {
        ext = ext + 1;
    }
    if (ext[0] == 0) {
        return FileType::Unknown;
    }
    if (strcmp(ext,"gz") == 0) { return FileType::GZ; }
    if (strcmp(ext,"gzip") == 0) { return FileType::GZ; }
    if (strcmp(ext,"fa") == 0) { return FileType::FASTA; }
    if (strcmp(ext,"fna") == 0) { return FileType::FASTA; }
    if (strcmp(ext,"ffn") == 0) { return FileType::FASTA; }
    if (strcmp(ext,"faa") == 0) { return FileType::FASTA; }
    if (strcmp(ext,"frn") == 0) { return FileType::FASTA; }
    if (strcmp(ext,"fasta") == 0) { return FileType::FASTA; }
    if (strcmp(ext,"fq") == 0) { return FileType::FASTQ; }
    if (strcmp(ext,"fastq") == 0) { return FileType::FASTQ; }
    if (strcmp(ext,"nsf") == 0) { return FileType::NSF; }
    return FileType::Unknown;
}
