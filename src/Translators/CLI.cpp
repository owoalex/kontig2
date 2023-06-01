#include <stdint.h>
#include <stdlib.h>
#include <cstring>
#include <stdio.h>
#include "CLI.h"
#include "FASTQReader.h"
#include "../Abstractions/NSF.h"
#include "../Utils/TypeDetector.h"
#include <iostream>

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
    
    Utils::FileType inputFileType = Utils::TypeDetector::file_extension_to_filetype(Utils::TypeDetector::extract_file_extension(inputFile));
    Utils::FileType outputFileType = Utils::TypeDetector::file_extension_to_filetype(Utils::TypeDetector::extract_file_extension(outputFile));
    

    //std::cout << inputFile << " --> " << outputFile << "\n";
    
    std::ifstream* inputStream = new std::ifstream(inputFile, std::ios_base::binary);
    std::ofstream* outputStream = new std::ofstream(outputFile, std::ios_base::binary);
    
    if (inputStream->fail()) {
        std::cout << "Couldn't read from " << inputFile << "\n";
        exit(1);
    }
    
    if (outputStream->fail()) {
        std::cout << "Couldn't write to " << outputFile << "\n";
        exit(1);
    }
    
    switch (outputFileType) {
        case Utils::FileType::Unknown:
            std::cout << "Couldn't determine the file type of " << outputFile << "\n";
            exit(1);
            break;
        case Utils::FileType::GZ:
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
        case Utils::FileType::Unknown:
            std::cout << "Unknown filetype " << inputFile << "\n";
            exit(1);
            break;
        case Utils::FileType::GZ:
            std::cout << "Kontig does not natively support gzipped files\n";
            exit(1);
            break;
        case Utils::FileType::FASTA:
            switch (outputFileType) {
                case Utils::FileType::NSF:
                    std::cout << "FASTA --> NSF\n";
                    exit(0);
                    break;
                default:
                    break;
            }
            break;
        case Utils::FileType::FASTQ:
            switch (outputFileType) {
                case Utils::FileType::NSF: {
                    Translators::FASTQReader* ifr = new Translators::FASTQReader(inputStream);
                    ifr->toNSF(outputStream);
                    exit(0);
                    break;
                }
                default:
                    break;
            }
            break;
        case Utils::FileType::NSF:
            switch (outputFileType) {
                case Utils::FileType::FASTA: {
                    Abstractions::NSF* nsf = new Abstractions::NSF(inputFile);
                    nsf->toFASTA(outputStream);
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
    std::cout << "No method to translate " << Utils::TypeDetector::extract_file_extension(inputFile) << " to " << Utils::TypeDetector::extract_file_extension(outputFile) << "\n";
    exit(1);
}
