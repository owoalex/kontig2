#include <stdint.h>
#include <stdlib.h>
#include <cstring>
#include <stdio.h>
#include "CLI.h"
#include "FASTQReader.h"
#include <iostream>

char* Translators::CLI::extract_file_extension(char* filename) {
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

Translators::FileType Translators::CLI::file_extension_to_filetype(char* ext) {
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
    if (strcmp(ext,"fasta") == 0) { return FileType::FASTA; }
    if (strcmp(ext,"fq") == 0) { return FileType::FASTQ; }
    if (strcmp(ext,"fastq") == 0) { return FileType::FASTQ; }
    if (strcmp(ext,"nsf") == 0) { return FileType::NSF; }
    if (strcmp(ext,"nsf") == 0) { return FileType::NSF; }
    return FileType::Unknown;
}

Translators::CLI::CLI(int argc, char** argv) {
    char* inputFile = NULL;
    char* outputFile = NULL;
    bool helpNeeded = false;
    
    for (int i = 2; i < argc; ++i) {
        char* arg = argv[i];
        if (strcmp(arg,"--input") == 0 || strcmp(arg,"-i") == 0) {
            i++;
            if (i < argc) {
                inputFile = argv[i];
            }
            continue;
        }
        if (strcmp(arg,"--output") == 0 || strcmp(arg,"-o") == 0) {
            i++;
            if (i < argc) {
                outputFile = argv[i];
            }
            continue;
        }
        if (strcmp(arg,"-h") == 0 || strcmp(arg,"--help") == 0) {
            helpNeeded = true;
            continue;
        }
        std::cout << "Unknown flag " << arg << "\n\n";
        helpNeeded = true;
        break;
    }
    
    if (inputFile == NULL) {
        std::cout << "Missing input file, please specify one with -i <file_path>\n";
        helpNeeded = true;
    }
    
    if (outputFile == NULL) {
        std::cout << "Missing output file, please specify one with -o <file_path>\n";
        helpNeeded = true;
    }
    
    if (helpNeeded) {
        std::cout << "Usage: kontig convert -i <input_file> -o <output_file> [options and flags]\n";
        std::cout << "\n";
        std::cout << "Options and flags\n";
        std::cout << "  -i, --input\n";
        std::cout << "  Specify an input file\n";
        std::cout << "  -o, --output\n";
        std::cout << "  Specify an output file\n";
        std::cout << "\n";
        std::cout << "Full documentation <https://kontig.alexbaldwin.dev/docs/convert>\n";
        exit(0);
    }
    
    Translators::FileType inputFileType = file_extension_to_filetype(extract_file_extension(inputFile));
    Translators::FileType outputFileType = file_extension_to_filetype(extract_file_extension(outputFile));
    

    //std::cout << inputFile << " --> " << outputFile << "\n";
    
    switch (outputFileType) {
        case Unknown:
            std::cout << "Couldn't determine the file type of " << outputFile << "\n";
            exit(1);
            break;
        case GZ:
            std::cout << "Kontig does not natively support gzipped files\n";
            exit(1);
            break;
        default:
            break;
    }
    
    if (outputFileType == inputFileType) {
        std::cout << "Query doesn't make sense: Desired output format appears to match the input format\n";
        exit(1);
    }
    
    switch (inputFileType) {
        case Unknown:
            std::cout << "Unknown filetype " << inputFile << "\n";
            exit(1);
            break;
        case GZ:
            std::cout << "Kontig does not natively support gzipped files\n";
            exit(1);
            break;
        case FASTA:
            switch (outputFileType) {
                case NSF:
                    std::cout << "FASTA --> NSF\n";
                    exit(0);
                    break;
                default:
                    break;
            }
            break;
        case FASTQ:
            switch (outputFileType) {
                case NSF: {
                    Translators::FASTQReader* ifr = new Translators::FASTQReader(new std::ifstream(inputFile, std::ios_base::binary));
                    ifr->toNSF(new std::ofstream(outputFile, std::ios_base::binary));
                    exit(0);
                    break;
                }
                default:
                    break;
            }
            break;
        default:
            break;
    }
    std::cout << "No method to translate " << extract_file_extension(inputFile) << " to " << extract_file_extension(outputFile) << "\n";
    exit(1);
}
