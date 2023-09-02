#include <stdint.h>
#include <stdlib.h>
#include <cstring>
#include <stdio.h>
#include <sys/stat.h>
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
    }
}

uint8_t Abstractions::NSF::streamCount() {
    this->fileStream->seekp(4);
    char count;
    this->fileStream->read(&count, 1);
    return count;
}

uint64_t Abstractions::NSF::entryCount(int streamId) {
    this->fileStream->seekp(HEADER_SIZE + (streamId * INDEX_ENTRY_SIZE) + 0);
    uint64_t count;
    this->fileStream->read(reinterpret_cast<char*>(&count), sizeof(count));
    return count;
}

uint64_t Abstractions::NSF::dataLength(int streamId, int entryId) {
    std::cout << "NOT IMPLEMENTED\n";
    exit(1);
}

char* Abstractions::NSF::rawDataPointer(int streamId, int entryId) {
    std::cout << "NOT IMPLEMENTED\n";
    exit(1);
}

void Abstractions::NSF::toFASTA(std::ofstream* outputStream) {
    if (outputStream->fail()) {
        std::cout << "Couldn't write to output file" << "\n";
        exit(1);
    }
    
    std::cout << (int) this->streamCount() << " streams to write\n";
    std::cout << (int) this->entryCount(0) << " entries to write\n";
    
    std::cout << "NOT IMPLEMENTED\n";
    exit(1);
}
