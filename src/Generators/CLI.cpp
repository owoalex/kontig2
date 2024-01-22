#include <stdint.h>
#include <stdlib.h>
#include <cstring>
#include <stdio.h>
#include "CLI.h"
#include "../Utils/TypeDetector.h"
#include "KLTBuilder.h"
#include <iostream>

Generators::CLI::CLI(int argc, char** argv) {
    char* inputFile = NULL;
    bool helpNeeded = false;
    
    uint8_t generateMode = 0;
    
    for (int i = 2; i < argc; ++i) {
        char* arg = argv[i];
        if (strcmp(arg,"--nsf") == 0 || strcmp(arg,"-i") == 0) {
            i++;
            if (i < argc) {
                inputFile = argv[i];
            }
            continue;
        }
        if (generateMode == 0) {
            if (strcmp(arg,"kmers") == 0) {
                generateMode = 1;
                continue;
            }
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
        std::cout << "Missing nsf file, please specify one with --nsf <file_path>\n";
        helpNeeded = true;
    }
    
    if (generateMode == 0) {
        std::cout << "Missing generation type, please select a valid generator type\n";
        helpNeeded = true;
    }
    
    if (helpNeeded) {
        std::cout << "Usage: kontig generate <type> --nsf <nsf_file> [options and flags]\n";
        std::cout << "\n";
        std::cout << "Global options and flags\n";
        std::cout << "  -i, --nsf\n";
        std::cout << "  Specify a nucleotide scratch file\n";
        std::cout << "\n";
        std::cout << "Generator types\n";
        std::cout << "  kmers\n";
        std::cout << "  Generates k-mer lookup table\n";
        std::cout << "\n";
        std::cout << "Full documentation <https://kontig.alexbaldwin.dev/docs/generate>\n";
        exit(0);
    }
    
    Utils::FileType inputFileType = Utils::TypeDetector::file_extension_to_filetype(Utils::TypeDetector::extract_file_extension(inputFile));
    

    //std::cout << inputFile << " --> " << outputFile << "\n";
    
    switch (inputFileType) {
        case Utils::FileType::Unknown:
            std::cout << "Unknown filetype " << inputFile << "\n";
            exit(1);
            break;
        case Utils::FileType::GZ:
            std::cout << "Kontig does not natively support gzipped files\n";
            exit(1);
            break;
        default:
            break;
    }
    
    switch (generateMode) {
        case 1: {
            Abstractions::NSF* nsf = new Abstractions::NSF(inputFile);
            KLTBuilder* builder = new KLTBuilder();
            builder->generate(nsf);
            exit(0);
            break;
        }
        default:
            std::cout << "Unimplemented generation mode\n";
            exit(1);
            break;
    }
}
