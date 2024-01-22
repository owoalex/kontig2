#include <stdint.h>
#include <stdlib.h>
#include <cstring>
#include <stdio.h>
#include <sys/stat.h>
#include <bit>
#include "NSF.h"
#include <vector>

Abstractions::NSF::NSF(char* filePath) {
    this->filePath = filePath;
    
    bool createNew = false;
    
    struct stat buffer; 
    if (!(stat(filePath, &buffer) == 0)) {
        std::cout << "Creating new NSF file at " << filePath << "\n";
        createNew = true;
    }
    
    this->fileStream = new std::fstream(this->filePath, std::ios_base::binary | std::ios::out | std::ios::in ); // Open for binary read/write
    
    if (this->fileStream->fail()) {
        std::cout << "Couldn't read/write from " << filePath << "\n";
        exit(1);
    }
    
    if (createNew) {
        this->fileStream->write(MAGIC_NUMBER, 8);
    } else {
        this->streamCount = getUInt64AtOffset(8);
        this->streamIndexOffsets = (int*) malloc(this->streamCount * sizeof(int));
        this->streamEntryCounts = (int*) malloc(this->streamCount * sizeof(int));
        this->streamTypes = (uint8_t*) malloc(this->streamCount * sizeof(uint8_t));
        this->streamIndexSizes = (uint8_t*) malloc(this->streamCount * sizeof(uint8_t));
        
        for (int i = 0; i < this->streamCount; i++) {
            this->streamIndexOffsets[i] = getUInt64AtOffset(ROOT_HEADER_SIZE + (i * STREAM_HEADER_SIZE) + 32);
            this->streamEntryCounts[i] = getUInt64AtOffset(ROOT_HEADER_SIZE + (i * STREAM_HEADER_SIZE) + 40);
            this->streamTypes[i] = getUInt8AtOffset(ROOT_HEADER_SIZE + (i * STREAM_HEADER_SIZE) + 16);
            this->streamIndexSizes[i] = getUInt8AtOffset(ROOT_HEADER_SIZE + (i * STREAM_HEADER_SIZE) + 17);
        }
    }
}

uint64_t Abstractions::NSF::getUInt64AtOffset(int offset) {
    this->fileStream->seekp(offset);
    uint64_t value;
    this->fileStream->read(reinterpret_cast<char*>(&value), sizeof(value));
    if constexpr(std::endian::native != std::endian::little) {
        value = std::byteswap(value);
    }
    return value;
}

uint8_t Abstractions::NSF::getUInt8AtOffset(int offset) {
    this->fileStream->seekp(offset);
    uint8_t value;
    this->fileStream->read(reinterpret_cast<char*>(&value), sizeof(value));
    return value;
}

uint8_t Abstractions::NSF::getStreamCount() {
    return this->streamCount;
}

uint64_t Abstractions::NSF::getEntryCount(int streamId) {
    return this->streamEntryCounts[streamId];
}

uint8_t Abstractions::NSF::getStreamType(int streamId) {
    return this->streamTypes[streamId];
}

uint64_t Abstractions::NSF::dataOffset(int streamId, int entryId) {
    return getUInt64AtOffset(this->streamIndexOffsets[streamId] + (entryId * this->streamIndexSizes[streamId]));
}

uint64_t Abstractions::NSF::dataLength(int streamId, int entryId) {
    return getUInt64AtOffset(this->streamIndexOffsets[streamId] + (entryId * this->streamIndexSizes[streamId]) + 8);
}

int Abstractions::NSF::getFirstStreamOfType(uint8_t type) {
    for (int i = 0; i < this->streamCount; i++) {
        if (this->streamTypes[i] == type) {
            return i;
        }
    }
    return -1;
}

char* Abstractions::NSF::getData(int streamId, int entryId) {
    uint64_t offset = getUInt64AtOffset(this->streamIndexOffsets[streamId] + (entryId * this->streamIndexSizes[streamId]));
    uint64_t length = getUInt64AtOffset(this->streamIndexOffsets[streamId] + (entryId * this->streamIndexSizes[streamId]) + 8);
    char* buffer = (char*) malloc(length);
    this->fileStream->seekp(offset);
    this->fileStream->read(buffer, length);
    return buffer;
}

char Abstractions::NSF::nsfToFastaChar(unsigned char c) {
    switch (c) {
        case 0b10000000:
            return 'A';
        case 0b01000000:
            return 'T';
        case 0b01000001:
            return 'U';
        case 0b00100000:
            return 'C';
        case 0b00010000:
            return 'G';
        case 0b00000001:
            return 'N';
        case 0b10010000:
            return 'R';
        case 0b01100000:
            return 'Y';
        case 0b01010000:
            return 'K';
        case 0b10100000:
            return 'M';
        case 0b00110000:
            return 'S';
        case 0b11000000:
            return 'W';
        case 0b01110000:
            return 'B';
        case 0b11010000:
            return 'D';
        case 0b11100000:
            return 'H';
        case 0b10110000:
            return 'V';
        case 0b11100001:
            return 'I';
        default:
            return '-';
    }
}

void Abstractions::NSF::toFASTA(std::ofstream* outputStream) {
    if (outputStream->fail()) {
        std::cerr << "Couldn't write to output file" << "\n";
        exit(1);
    }
    
    int nucleotideStream = getFirstStreamOfType(NUCLEOTIDE_STREAM_IDENTIFIER);
    int qualityStream = getFirstStreamOfType(QUALITY_STREAM_IDENTIFIER);
    int tagStream = getFirstStreamOfType(TAG_STREAM_IDENTIFIER);
    
    if (nucleotideStream == -1) {
        std::cerr << "NSF is missing nucleotide stream" << "\n";
        exit(1);
    }
    
    if (tagStream == -1) {
        std::cout << "NSF is missing tag stream, generating based on index" << "\n";
    }

    outputStream->seekp(0);
    char* inputBuffer = (char*) malloc(80);
    char* tagInputBuffer;
    for (int i = 0; i < this->getEntryCount(nucleotideStream); i++) {
        uint64_t lengthRemain = this->dataLength(nucleotideStream, i);
        uint64_t offset = this->dataOffset(nucleotideStream, i);
        //char* data = this->getData(0, i);
        //std::cout << "Read " << (int) lengthRemain << "\n";
        if (tagStream == -1) {
            *outputStream << ">R" << (int) i << "\n";
        } else {
            int tagLength = this->dataLength(tagStream, i);
            tagInputBuffer = (char*) malloc(tagLength + 1);
            this->fileStream->seekp(this->dataOffset(tagStream, i));
            this->fileStream->read(tagInputBuffer, tagLength);
            tagInputBuffer[tagLength] = 0;
            *outputStream << ">" << tagInputBuffer << "\n";
            free(tagInputBuffer);
        }
        //const char* outputBlock
        //outputStream->write(outputBlock, currentOutputBlockPosition);
        int lineLength = 0;
        while (lengthRemain > 0) {
            if (lengthRemain > 80) {
                lengthRemain -= 80;
                lineLength = 80;
            } else {
                lineLength = lengthRemain;
                lengthRemain = 0;
            }
            this->fileStream->seekp(offset);
            this->fileStream->read(inputBuffer, lineLength);
            offset += lineLength;
            for (int i = 0; i < lineLength; i++) {
                *outputStream << nsfToFastaChar(inputBuffer[i]);
            }
            *outputStream << "\n";
        }
        
    }
    free(inputBuffer);
    outputStream->flush();
    
    
    std::cout << (int) this->getEntryCount(nucleotideStream) << " scaffolds converted\n";
}

void Abstractions::NSF::toFASTQ(std::ofstream* outputStream) {
    if (outputStream->fail()) {
        std::cerr << "Couldn't write to output file" << "\n";
        exit(1);
    }
    
    int nucleotideStream = getFirstStreamOfType(NUCLEOTIDE_STREAM_IDENTIFIER);
    int qualityStream = getFirstStreamOfType(QUALITY_STREAM_IDENTIFIER);
    int tagStream = getFirstStreamOfType(TAG_STREAM_IDENTIFIER);
    
    if (nucleotideStream == -1) {
        std::cerr << "NSF is missing nucleotide stream" << "\n";
        exit(1);
    }
    
    if (qualityStream == -1) {
        std::cout << "NSF is missing quality stream" << "\n";
        exit(1);
    }
    
    if (tagStream == -1) {
        std::cout << "NSF is missing tag stream, generating based on index" << "\n";
    }
    
    //std::cout << (int) this->getStreamCount() << " streams to write\n";
    //std::cout << (int) this->getEntryCount(nucleotideStream) << " entries to write\n";
    outputStream->seekp(0);
    char* inputBuffer = (char*) malloc(80);
    char* tagInputBuffer;
    for (int i = 0; i < this->getEntryCount(nucleotideStream); i++) {
        uint64_t lengthRemain = this->dataLength(nucleotideStream, i);
        uint64_t offset = this->dataOffset(nucleotideStream, i);
        //char* data = this->getData(0, i);
        //std::cout << "Read " << (int) lengthRemain << "\n";
        if (tagStream == -1) {
            *outputStream << "@R" << (int) i << "\n";
        } else {
            int tagLength = this->dataLength(tagStream, i);
            tagInputBuffer = (char*) malloc(tagLength + 1);
            this->fileStream->seekp(this->dataOffset(tagStream, i));
            this->fileStream->read(tagInputBuffer, tagLength);
            tagInputBuffer[tagLength] = 0;
            *outputStream << "@" << tagInputBuffer << "\n";
        }
        //const char* outputBlock
        //outputStream->write(outputBlock, currentOutputBlockPosition);
        int lineLength = 0;
        while (lengthRemain > 0) {
            if (lengthRemain > 80) {
                lengthRemain -= 80;
                lineLength = 80;
            } else {
                lineLength = lengthRemain;
                lengthRemain = 0;
            }
            this->fileStream->seekp(offset);
            this->fileStream->read(inputBuffer, lineLength);
            offset += lineLength;
            for (int i = 0; i < lineLength; i++) {
                *outputStream << nsfToFastaChar(inputBuffer[i]);
            }
            *outputStream << "\n";
        }
        
        // Now do quality data
        
        lengthRemain = this->dataLength(qualityStream, i);
        offset = this->dataOffset(qualityStream, i);
        //char* data = this->getData(0, i);
        //std::cout << "Read " << (int) lengthRemain << "\n";
        if (tagStream == -1) {
            *outputStream << "+R" << (int) i << "\n";
        } else {
            *outputStream << "+" << tagInputBuffer << "\n";
            free(tagInputBuffer);
        }
        //const char* outputBlock
        //outputStream->write(outputBlock, currentOutputBlockPosition);
        lineLength = 0;
        while (lengthRemain > 0) {
            if (lengthRemain > 80) {
                lengthRemain -= 80;
                lineLength = 80;
            } else {
                lineLength = lengthRemain;
                lengthRemain = 0;
            }
            this->fileStream->seekp(offset);
            this->fileStream->read(inputBuffer, lineLength);
            offset += lineLength;
            for (int i = 0; i < lineLength; i++) {
                if (inputBuffer[i] > 93) {
                    inputBuffer[i] = 93; // We're limited to the FASTQ spec, this is the max Phred score storable
                }
                *outputStream << (char) (inputBuffer[i] + 33); // Shift into ASCII range
            }
            *outputStream << "\n";
        }
    }
    free(inputBuffer);
    outputStream->flush();
    
    std::cout << (int) this->getEntryCount(nucleotideStream) << " scaffolds converted\n";
}
