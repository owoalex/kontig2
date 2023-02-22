#include <stdint.h>
#include <stdlib.h>
#include <cstring>
#include <stdio.h>
#include "CLI.h"
#include <iostream>

Translators::CLI::CLI(int argc, char** argv) {
    char* inputFile = NULL;
    char* outputFile = NULL;
    bool helpNeeded = false;
    
    for (int i = 2; i < argc; ++i) {
        char* arg = argv[i];
        if (strcmp(arg,"--input") == 0 || strcmp(arg,"-i") == 0) {
            i++;
            inputFile = argv[i];
            continue;
        }
        if (strcmp(arg,"--output") == 0 || strcmp(arg,"-o") == 0) {
            i++;
            outputFile = argv[i];
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
    
    if (inputFile == NULL) {
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
        std::cout << "Full documentation <https://kontig.alexbaldwin.dev/docs>\n";
        exit(0);
    }

    std::cout << inputFile << " --> " << outputFile << "\n";
}
