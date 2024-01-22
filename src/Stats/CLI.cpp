#include <stdint.h>
#include <stdlib.h>
#include <cstring>
#include <stdio.h>
#include "CLI.h"
#include "../Abstractions/NSF.h"
#include "../Utils/TypeDetector.h"
#include <iostream>

Stats::CLI::CLI(int argc, char** argv) {
    char* inputFile = NULL;
    char* outputFile = NULL;
    bool helpNeeded = false;
    
    for (int i = 2; i < argc; ++i) {
        char* arg = argv[i];
        if (strcmp(arg,"-h") == 0 || strcmp(arg,"--help") == 0) {
            helpNeeded = true;
            continue;
        }
        if (arg[0] == '-') {
            std::cerr << "Unknown flag " << arg << "\n\n";
            helpNeeded = true;
            break;
        } else {
            if (inputFile == NULL) {
                inputFile = argv[i];
            } else {
                std::cerr << "Only one file is permitted at once, token \"" << arg << "\" is ambiguous\n\n";
                helpNeeded = true;
                break;
            }
        }
    }
    
    if (!helpNeeded) {
        if (inputFile == NULL) {
            std::cerr << "Missing input file, please specify one\n";
            helpNeeded = true;
        }
    }
    
    if (helpNeeded) {
        std::cout << "Usage: kontig stats <input_file> [options and flags]\n";
        std::cout << "\n";
        std::cout << "Options and flags\n";
        std::cout << "  None applicable";
        std::cout << "\n";
        std::cout << "Full documentation <https://kontig.alexbaldwin.dev/docs/stats>\n";
        exit(0);
    }
    
    Utils::FileType inputFileType = Utils::TypeDetector::file_extension_to_filetype(Utils::TypeDetector::extract_file_extension(inputFile));
    
    //std::cout << inputFile << " --> " << outputFile << "\n";
    
    std::ifstream* inputStream = new std::ifstream(inputFile, std::ios_base::binary);
    
    if (inputStream->fail()) {
        std::cerr << "Couldn't read from " << inputFile << "\n";
        exit(1);
    }
    
    switch (inputFileType) {
        case Utils::FileType::Unknown:
            std::cerr << "Unknown filetype " << inputFile << "\n";
            exit(1);
            break;
        case Utils::FileType::GZ:
            std::cerr << "This version of kontig does not natively support gzipped files\n";
            exit(1);
            break;
        case Utils::FileType::NSF: {
                Abstractions::NSF* nsf = new Abstractions::NSF(inputFile);
                int streamCount = nsf->getStreamCount();
                std::cout << "File contains " << streamCount << " streams\n";
                int emptyStreams = 0;
                for (int i = 0; i < streamCount; i++) {
                    uint8_t streamType = nsf->getStreamType(i);
                    if (streamType == 0) {
                        emptyStreams++;
                    } else {
                        if (emptyStreams > 0) {
                            if (emptyStreams > 1) {
                                std::cout << "[" << (i - emptyStreams) << "-" << i - 1 << "] Empty streams\n";
                            } else {
                                std::cout << "[" << i - 1 << "] Empty stream\n";
                            }
                        }
                        emptyStreams = 0;
                        std::cout << "[" << i << "] ";
                        switch (streamType) {
                            case Abstractions::NSF::FRAGMENT_RECORD_STREAM_IDENTIFIER:
                                std::cout << "Fragment record (Metadata)";
                                break;
                            case Abstractions::NSF::NUCLEOTIDE_STREAM_IDENTIFIER:
                                std::cout << "Base call nucleotide data";
                                break;
                            case Abstractions::NSF::QUALITY_STREAM_IDENTIFIER:
                                std::cout << "Base call quality data";
                                break;
                            case Abstractions::NSF::TAG_STREAM_IDENTIFIER:
                                std::cout << "String data (Tags)";
                                break;
                            case Abstractions::NSF::KMER_TREE_LL_STREAM_IDENTIFIER:
                                std::cout << "KMer tree (Linked list)";
                                break;
                            case Abstractions::NSF::KMER_TREE_ARR_STREAM_IDENTIFIER:
                                std::cout << "KMer tree (Array)";
                                break;
                        }
                        std::cout << "\n";
                    }
                }
                if (emptyStreams > 0) {
                    if (emptyStreams > 1) {
                        std::cout << "[" << (streamCount - emptyStreams) << "-" << streamCount - 1 << "] Empty streams\n";
                    } else {
                        std::cout << "[" << streamCount - 1 << "] Empty stream\n";
                    }
                }
                exit(0);
            }
            break;
        default:
            break;
    }
    std::cerr << "Unsupported filetype " << Utils::TypeDetector::extract_file_extension(inputFile) << "\n";
    exit(1);
}
