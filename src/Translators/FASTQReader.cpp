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
        //currentOffset = this->inputStream->tellg();
        charsRead = 1024;
        if ((currentOffset + charsRead) >= totalFileSize) {
            charsRead = totalFileSize - currentOffset;
        }
        this->inputStream->read(inputBuffer, charsRead);
        charsRead = this->inputStream->gcount();
        
        //std::cout << "READ #" << charsRead << "\n";
        
        if (skipCount < charsRead) {
            //std::cout << "START " << skipCount << "\n";
            int tmpSkipCount = skipCount;
            skipCount = 0;
            for (int i = tmpSkipCount; i < charsRead; i++) {
                if (inputBuffer[i] == '\0') {
                    break;
                }
                switch (state) {
                    case 3: {
                        if (inputBuffer[i] == '\n') {
                            scaffoldQualityDataOffsets.push_back(currentOffset + i + 1);
                            //std::cout << "Qual @" << currentOffset + i + 1 << "\n";
                            state = -1;
                            skipCount = currentScaffoldLength;
                            //std::cout << "SKPTO " << currentOffset + i + 1 + skipCount << " ";
                            if (skipCount > (charsRead - i)) {
                                skipCount -= (charsRead - i);
                                i = charsRead;
                                //std::cout << "NEXT\n";
                            } else {
                                i += skipCount - 1; // -1 to account for the inhernet skip forward in the loop
                                skipCount = 0;
                                //std::cout << "MID\n";
                            }
                        }
                        break;
                    }
                    case 2: {
                        if (inputBuffer[i] == '+') {
                            state = 3;
                            scaffoldGenomicDataLengths.push_back(currentScaffoldLength);
                            //std::cout << "Len  #" << currentScaffoldLength << "\n";
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
                            scaffoldGenomicDataOffsets.push_back(currentOffset + i + 1);
                            //std::cout << "Gnom @" << currentOffset + i + 1 << "\n";
                            state = 2;
                        }
                        break;
                    }
                    case 0: {
                        if (inputBuffer[i] == '@') {
                            scaffoldOffsets.push_back(currentOffset + i);
                            //std::cout << "Scaf @" << currentOffset + i << "\n";
                            scaffoldNameOffsets.push_back(currentOffset + i + 1);
                            currentScaffoldLength = 0;
                            currentScaffoldNucleotides = 0;
                            state = 1;
                        }
                        break;
                    }
                    default: {
                        state = 0;
                        break;
                    }
                    
                }
            }
        } else {
            skipCount -= charsRead;
        }
        currentOffset += charsRead;
    } while (charsRead == 1024);
    
    char* outputBlock = (char*) malloc(sizeof(char) * 1024);
    
    int headerTail = 128;
    const int INDEX_ENTRY_SIZE = 64;
    int nucleotideStreamOffset = headerTail + (scaffoldOffsets.size() * INDEX_ENTRY_SIZE);
    int nucleotideStreamTail = nucleotideStreamOffset;
    
    this->inputStream->seekg(0, std::ios_base::beg);
    //int t = this->inputStream->tellg();
    //std::string str = std::to_string(t);
    //const char* cstr = str.c_str();
    
    const char magicNumber[4] = {0x62, 0x6e, 0x61, 0x00};
    outputStream->seekp(0);
    outputStream->write(magicNumber, 4);
    
    for (std::vector<uint64_t>::size_type i = 0; i < scaffoldOffsets.size(); i++) {
        int leftToRead = scaffoldGenomicDataLengths[i];
        int leftToWrite = scaffoldGenomicDataNucleotides[i];
        this->inputStream->seekg(scaffoldGenomicDataOffsets[i]);
        
        //t = scaffoldGenomicDataOffsets[i];
        //std::string str = std::to_string(t);
        //const char* cstr = str.c_str();
        //std::cout << "Starting from " << cstr << "\n";
        
        //t = scaffoldGenomicDataLengths[i];
        //str = std::to_string(t);
        //cstr = str.c_str();
        //std::cout << "Length " << cstr << "\n";
        
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
                            outputBlock[currentOutputBlockPosition] = 0b10000000;
                            currentOutputBlockPosition++;
                            break;
                        case 'T':
                        case 't':
                            outputBlock[currentOutputBlockPosition] = 0b01000000;
                            currentOutputBlockPosition++;
                            break;
                        case 'U':
                        case 'u':
                            outputBlock[currentOutputBlockPosition] = 0b01000001;
                            currentOutputBlockPosition++;
                            break;
                        case 'C':
                        case 'c':
                            outputBlock[currentOutputBlockPosition] = 0b00100000;
                            currentOutputBlockPosition++;
                            break;
                        case 'G':
                        case 'g':
                            outputBlock[currentOutputBlockPosition] = 0b00010000;
                            currentOutputBlockPosition++;
                            break;
                        case 'N':
                        case 'n':
                            outputBlock[currentOutputBlockPosition] = 0b00000001;
                            currentOutputBlockPosition++;
                            break;
                        case '-':
                            outputBlock[currentOutputBlockPosition] = 0b00000011;
                            currentOutputBlockPosition++;
                            break;
                        case 'R':
                        case 'r':
                            outputBlock[currentOutputBlockPosition] = 0b10010000;
                            currentOutputBlockPosition++;
                            break;
                        case 'Y':
                        case 'y':
                            outputBlock[currentOutputBlockPosition] = 0b01100000;
                            currentOutputBlockPosition++;
                            break;
                        case 'K':
                        case 'k':
                            outputBlock[currentOutputBlockPosition] = 0b01010000;
                            currentOutputBlockPosition++;
                            break;
                        case 'M':
                        case 'm':
                            outputBlock[currentOutputBlockPosition] = 0b10100000;
                            currentOutputBlockPosition++;
                            break;
                        case 'S':
                        case 's':
                            outputBlock[currentOutputBlockPosition] = 0b00110000;
                            currentOutputBlockPosition++;
                            break;
                        case 'W':
                        case 'w':
                            outputBlock[currentOutputBlockPosition] = 0b11000000;
                            currentOutputBlockPosition++;
                            break;
                        case 'B':
                        case 'b':
                            outputBlock[currentOutputBlockPosition] = 0b01110000;
                            currentOutputBlockPosition++;
                            break;
                        case 'D':
                        case 'd':
                            outputBlock[currentOutputBlockPosition] = 0b11010000;
                            currentOutputBlockPosition++;
                            break;
                        case 'H':
                        case 'h':
                            outputBlock[currentOutputBlockPosition] = 0b11100000;
                            currentOutputBlockPosition++;
                            break;
                        case 'V':
                        case 'v':
                            outputBlock[currentOutputBlockPosition] = 0b10110000;
                            currentOutputBlockPosition++;
                            break;
                        case 'i':
                        case 'I':
                            outputBlock[currentOutputBlockPosition] = 0b11100001;
                            currentOutputBlockPosition++; // NOTE: Non-standard, more testing to be done on how this affects quality of assembled contigs
                            break;
                        default:
                            outputBlock[currentOutputBlockPosition] = 0b00000000;
                            currentOutputBlockPosition++;
                            break;
                    }
                }
            }
            /*
             * ATCG---A
             * ^^^^      Standard 4 bases
             *       ^   Unknown length, commonly paired with 0000 on ATCG
             *        ^  Alternate interpretation
             *           e.g. 0000 on ATCG can be read as an "N"
             *                0100 on ATCG can be read as a "U" for a RNA scaffold
             * 
             *           All zeroes == end of scaffold
            
            
            leftToWrite -= currentOutputBlockPosition;
            if (currentOutputBlockPosition < 1024) {
                outputBlock[currentOutputBlockPosition] = '\0';
            }
            const char* outputBlockFinal = outputBlock;
            
            //std::string str = std::to_string(i+1);
            //const char* cstr = str.c_str();
            //std::cout << "Block # " << cstr << "\n";
            //std::cout << "BLOCK:[\n" << outputBlockFinal << "\n]\n";
            outputStream->seekp(nucleotideStreamTail);
            outputStream->write(outputBlockFinal, currentOutputBlockPosition);
            
            nucleotideStreamTail += currentOutputBlockPosition;
            leftToRead -= inputBlockSize;
        }
        
        //if (i > 8) {
        //    break;
        //}
    }
    
    int tagStreamOffset = nucleotideStreamTail;
    int tagStreamTail = tagStreamOffset;
    
    free(inputBuffer);
    free(outputBlock);
    
    std::cout << scaffoldOffsets.size() << " scaffolds converted\n";
}

Translators::FASTQReader::FASTQReader(std::ifstream* inputStream) {
    this->inputStream = inputStream;
}
