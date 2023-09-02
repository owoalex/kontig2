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
        this->fileStream->write(MAGIC_NUMBER, 4);
    } else {
        this->streamCount = getUInt64AtOffset(4);
        this->streamIndexOffsets = (int*) malloc(this->streamCount * sizeof(int));
        this->streamEntryCounts = (int*) malloc(this->streamCount * sizeof(int));
        this->streamTypes = (uint8_t*) malloc(this->streamCount * sizeof(uint8_t));
        this->streamIndexSizes = (uint8_t*) malloc(this->streamCount * sizeof(uint8_t));
        
        for (int i = 0; i < this->streamCount; i++) {
            this->streamIndexOffsets[i] = getUInt64AtOffset(HEADER_SIZE + (i * STREAM_HEADER_SIZE) + 0);
            this->streamEntryCounts[i] = getUInt64AtOffset(HEADER_SIZE + (i * STREAM_HEADER_SIZE) + 8);
            this->streamTypes[i] = getUInt8AtOffset(HEADER_SIZE + (i * STREAM_HEADER_SIZE) + 16);
            this->streamIndexSizes[i] = getUInt8AtOffset(HEADER_SIZE + (i * STREAM_HEADER_SIZE) + 17);
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

uint64_t Abstractions::NSF::dataOffset(int streamId, int entryId) {
    return getUInt64AtOffset(this->streamIndexOffsets[streamId] + (entryId * this->streamIndexSizes[streamId]));
}

uint64_t Abstractions::NSF::dataLength(int streamId, int entryId) {
    return getUInt64AtOffset(this->streamIndexOffsets[streamId] + (entryId * this->streamIndexSizes[streamId]) + 8);
}

char* Abstractions::NSF::getData(int streamId, int entryId) {
    uint64_t offset = getUInt64AtOffset(this->streamIndexOffsets[streamId] + (entryId * this->streamIndexSizes[streamId]));
    uint64_t length = getUInt64AtOffset(this->streamIndexOffsets[streamId] + (entryId * this->streamIndexSizes[streamId]) + 8);
    char* buffer = (char*) malloc(length);
    this->fileStream->seekp(offset);
    this->fileStream->read(buffer, length);
    return buffer;
}

void Abstractions::NSF::toFASTA(std::ofstream* outputStream) {
    if (outputStream->fail()) {
        std::cout << "Couldn't write to output file" << "\n";
        exit(1);
    }
    
    std::cout << (int) this->getStreamCount() << " streams to write\n";
    std::cout << (int) this->getEntryCount(0) << " entries to write\n";
    outputStream->seekp(0);
    for (int i = 0; i < this->getEntryCount(0); i++) {
        uint64_t lengthRemain = this->dataLength(0, i);
        uint64_t offset = this->dataOffset(0, i);
        //char* data = this->getData(0, i);
        //std::cout << "Read " << (int) lengthRemain << "\n";
        *outputStream << ">R" << (int) i << "\n";
        //const char* outputBlock
        //outputStream->write(outputBlock, currentOutputBlockPosition);
        int lineLength = 0;
        char* inputBuffer = (char*) malloc(80);
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
                switch ((unsigned char) inputBuffer[i]) {
                    case 0b10000000:
                        *outputStream << 'A';
                        break;
                    case 0b01000000:
                        *outputStream << 'T';
                        break;
                    case 0b01000001:
                        *outputStream << 'U';
                        break;
                    case 0b00100000:
                        *outputStream << 'C';
                        break;
                    case 0b00010000:
                        *outputStream << 'G';
                        break;
                    case 0b00000001:
                        *outputStream << 'N';
                        break; 
                    case 0b10010000:
                        *outputStream << 'R';
                        break;
                    case 0b01100000:
                        *outputStream << 'Y';
                        break;
                    case 0b01010000:
                        *outputStream << 'K';
                        break;
                    case 0b10100000:
                        *outputStream << 'M';
                        break;
                    case 0b00110000:
                        *outputStream << 'S';
                        break;
                    case 0b11000000:
                        *outputStream << 'W';
                        break;
                    case 0b01110000:
                        *outputStream << 'B';
                        break;
                    case 0b11010000:
                        *outputStream << 'D';
                        break;
                    case 0b11100000:
                        *outputStream << 'H';
                        break;
                    case 0b10110000:
                        *outputStream << 'V';
                        break;
                    case 0b11100001:
                        *outputStream << 'I';
                        break;
                    default:
                        *outputStream << '-';
                        break;
                }
            }
            *outputStream << "\n";
        }
        free(inputBuffer);
    }
    
}
