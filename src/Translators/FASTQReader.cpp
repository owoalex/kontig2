#include <stdint.h>
#include <stdlib.h>
#include <cstring>
#include <stdio.h>
#include "FASTQReader.h"
#include "FASTQReader.h"
#include <vector>

void Translators::FASTQReader::toNSF(std::ofstream* outputStream) {
    std::vector<uint64_t> scaffoldOffsets;
    std::vector<uint64_t> scaffoldNameOffsets;
    std::vector<uint64_t> scaffoldGenomicDataOffsets;
    std::vector<uint64_t> scaffoldGenomicDataLengths;
    std::vector<uint64_t> scaffoldGenomicDataNucleotides;
    std::vector<uint64_t> scaffoldQualityDataOffsets;
    
    char* inputBuffer = (char*) malloc(1024);
    int charsRead = 0;
    int state = 0;
    uint64_t currentOffset = 0;
    this->inputStream->seekg(0, std::ios::end);
    uint64_t totalFileSize = this->inputStream->tellg();
    this->inputStream->seekg(0);
    int currentScaffoldLength = 0;
    int currentScaffoldNucleotides = 0;
    int skipCount = 0;
    
    do {
        currentOffset = this->inputStream->tellg();
        charsRead = 1024;
        if ((currentOffset + charsRead) >= totalFileSize) {
            charsRead = totalFileSize - currentOffset;
        }
        this->inputStream->read(inputBuffer, charsRead);
        charsRead = this->inputStream->gcount();
        
        if (skipCount < charsRead) {
            for (int i = skipCount; i < charsRead; i++) {
                if (inputBuffer[i] == '\0') {
                    break;
                }
                switch (state) {
                    case 3: {
                        if (inputBuffer[i] == '\n') {
                            if (inputBuffer[i + 1] == '\r') {
                                i++;
                            }
                            scaffoldQualityDataOffsets.push_back(currentOffset + i + 1);
                            state = 0;
                            skipCount = currentScaffoldLength;
                            if (skipCount < (charsRead - i)) {
                                i = charsRead;
                                skipCount -= (charsRead - i);
                            } else {
                                i += skipCount;
                                skipCount = 0;
                            }
                        }
                        break;
                    }
                    case 2: {
                        if (inputBuffer[i] == '+') {
                            state = 3;
                            scaffoldGenomicDataLengths.push_back(currentScaffoldLength);
                            scaffoldGenomicDataNucleotides.push_back(currentScaffoldNucleotides);
                        } else if (((inputBuffer[i] >= 'a') && (inputBuffer[i] <= 'z')) || ((inputBuffer[i] >= 'A') && (inputBuffer[i] <= 'Z')) || (inputBuffer[i] == '-') || ((inputBuffer[i] >= '0') && (inputBuffer[i] <= '9'))) {
                            currentScaffoldNucleotides++;
                            currentScaffoldLength++;
                        } else {
                            currentScaffoldLength++;
                        }
                        break;
                    }
                    case 1: {
                        if (inputBuffer[i] == '\n') {
                            if (inputBuffer[i + 1] == '\r') {
                                i++;
                            }
                            scaffoldGenomicDataOffsets.push_back(currentOffset + i + 1);
                            state = 2;
                        }
                        break;
                    }
                    case 0: {
                        if (inputBuffer[i] == '@') {
                            scaffoldOffsets.push_back(currentOffset + i);
                            scaffoldNameOffsets.push_back(currentOffset + i + 1);
                            currentScaffoldLength = 0;
                            currentScaffoldNucleotides = 0;
                            state = 1;
                        }
                        break;
                    }
                }
            }
        }
        
        skipCount -= charsRead;
        //currentOffset += charsRead;
    } while (charsRead == 1024);
    
    char* outputBlock = (char*) malloc(sizeof(char) * 1024);
    
    int headerTail = 128;
    const int INDEX_ENTRY_SIZE = 64;
    int nucleotideStreamOffset = headerTail + (scaffoldOffsets.size() * INDEX_ENTRY_SIZE);
    int nucleotideStreamTail = nucleotideStreamOffset;
    
    this->inputStream->seekg(0, std::ios_base::beg);
    int t = this->inputStream->tellg();
    std::string str = std::to_string(t);
    const char* cstr = str.c_str();
    
    for (std::vector<uint64_t>::size_type i = 0; i < scaffoldOffsets.size(); i++) {
        int leftToRead = scaffoldGenomicDataLengths[i];
        int leftToWrite = scaffoldGenomicDataNucleotides[i];
        this->inputStream->seekg(scaffoldGenomicDataOffsets[i]);
        
        t = scaffoldGenomicDataOffsets[i];
        std::string str = std::to_string(t);
        const char* cstr = str.c_str();
        std::cout << "Starting from " << cstr << "\n";
        
        while (leftToRead > 0) {
            int inputBlockSize = leftToRead;
            if (inputBlockSize > 1024) {
                inputBlockSize = 1024;
            }

            this->inputStream->read(inputBuffer, inputBlockSize);
            inputBlockSize = this->inputStream->gcount();
            
            int currentOutputBlockPosition = 0;
            for (int j = 0; j < inputBlockSize; j++) {
                if (((inputBuffer[j] >= 'a') && (inputBuffer[j] <= 'z')) || ((inputBuffer[j] >= 'A') && (inputBuffer[j] <= 'Z')) || (inputBuffer[j] == '-') || ((inputBuffer[j] >= '0') && (inputBuffer[j] <= '9'))) {
                    switch (inputBuffer[j]) {
                        case 'A':
                        case 'a':
                            outputBlock[currentOutputBlockPosition] = 'A';
                            currentOutputBlockPosition++;
                            break;
                        case 'C':
                        case 'c':
                            outputBlock[currentOutputBlockPosition] = 'C';
                            currentOutputBlockPosition++;
                            break;
                        case 'T':
                        case 't':
                            outputBlock[currentOutputBlockPosition] = 'T';
                            currentOutputBlockPosition++;
                            break;
                        case 'G':
                        case 'g':
                            outputBlock[currentOutputBlockPosition] = 'G';
                            currentOutputBlockPosition++;
                            break;
                        default:
                            outputBlock[currentOutputBlockPosition] = '!';
                            currentOutputBlockPosition++;
                            break;
                    }
                }
            }
            
            leftToWrite -= currentOutputBlockPosition;
            if (currentOutputBlockPosition < 1024) {
                outputBlock[currentOutputBlockPosition] = '\0';
            }
            const char* outputBlockFinal = outputBlock;
            
            std::string str = std::to_string(i+1);
            const char* cstr = str.c_str();
            std::cout << "Block # " << cstr << "\n";
            std::cout << "BLOCK:[\n" << outputBlockFinal << "\n]\n";
            outputStream->seekp(nucleotideStreamTail);
            outputStream->write(outputBlockFinal, currentOutputBlockPosition);
            
            nucleotideStreamTail += currentOutputBlockPosition;
            leftToRead -= inputBlockSize;
        }
        
        if (i > 8) {
            break;
        }
    }
    
    int tagStreamOffset = nucleotideStreamTail;
    int tagStreamTail = tagStreamOffset;
    
    free(inputBuffer);
    free(outputBlock);
    
    str = std::to_string(scaffoldOffsets.size());
    cstr = str.c_str();
    std::cout << cstr << " scaffolds in file\n";
}

Translators::FASTQReader::FASTQReader(std::ifstream* inputStream) {
    this->inputStream = inputStream;
}