#include <stdint.h>
#include <stdlib.h>
#include <cstring>
#include <stdio.h>
#include "FASTQReader.h"
#include "FASTQReader.h"
#include <vector>

void Translators::FASTQReader::toNSF(char* filename) {
    std::vector<uint64_t> scaffoldOffsets;
    std::vector<uint64_t> scaffoldNameOffsets;
    std::vector<uint64_t> scaffoldGenomicDataOffsets;
    std::vector<uint64_t> scaffoldQualityDataOffsets;
    
    char* inputBuffer = (char*) malloc(1024);
    bool stillReadable = true;
    int charsRead = 0;
    int state = 0;
    uint64_t currentOffset = 0;
    this->inputStream->seekg(0, this->inputStream->beg);
    
    do {
        currentOffset = this->inputStream->tellg();
        
        this->inputStream->read(inputBuffer, 1024);
        charsRead = this->inputStream->gcount();
        stillReadable = charsRead > 0;
        
        for (int i = 0; i < charsRead; i++) {
            if (inputBuffer[i] == '\0') {
                break;
            }
            switch (state) {
                case 3: {
                    if (inputBuffer[i] == '\n') {
                        if (inputBuffer[i + 1] == '\r') {
                            scaffoldQualityDataOffsets.push_back(currentOffset + i + 2);
                        } else {
                            scaffoldQualityDataOffsets.push_back(currentOffset + i + 1);
                        }
                        state = 0;
                    }
                    break;
                }
                case 2: {
                    if (inputBuffer[i] == '+') {
                        state = 3;
                    }
                    break;
                }
                case 1: {
                    if (inputBuffer[i] == '\n') {
                        if (inputBuffer[i + 1] == '\r') {
                            scaffoldGenomicDataOffsets.push_back(currentOffset + i + 2);
                        } else {
                            scaffoldGenomicDataOffsets.push_back(currentOffset + i + 1);
                        }
                        state = 2;
                    }
                    break;
                }
                case 0:
                    if (inputBuffer[i] == '@') {
                        scaffoldOffsets.push_back(currentOffset + i);
                        scaffoldNameOffsets.push_back(currentOffset + i + 1);
                        state = 1;
                    }
                    break;
            }
        }
        
        //currentOffset += charsRead;
    } while (stillReadable);
    
    free(inputBuffer);
    
    for (std::vector<uint64_t>::size_type i = 0; i < 8; i++) {
        std::string str = std::to_string(scaffoldOffsets[i]);
        char const* cstr = str.c_str();
        std::cout << "Scaffold @" << cstr << "\n";
        
        str = std::to_string(scaffoldNameOffsets[i]);
        cstr = str.c_str();
        std::cout << "Name @" << cstr << "\n";
        
        str = std::to_string(scaffoldGenomicDataOffsets[i]);
        cstr = str.c_str();
        std::cout << "Genome @" << cstr << "\n";
        
        str = std::to_string(scaffoldQualityDataOffsets[i]);
        cstr = str.c_str();
        std::cout << "Quality @" << cstr << "\n";
    }
    
    std::string str = std::to_string(scaffoldOffsets.size());
    char const* cstr = str.c_str();
    std::cout << cstr << " scaffolds in file\n";
}

Translators::FASTQReader::FASTQReader(char* filename) {
    this->inputStream = new std::ifstream(filename);
}
