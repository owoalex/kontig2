#include <stdint.h>
#include <stdlib.h>
#include <cstring>
#include <stdio.h>
#include "NSF.h"
#include <vector>

Abstractions::NSF::NSF(char* filePath) {
    this->filePath = filePath;
    this->fileStream = new std::fstream(this->filePath, std::ios_base::binary | std::ios::out | std::ios::in ); // Open for binary read/write
    
    if (this->fileStream->fail()) {
        std::cout << "Couldn't read/write from " << filePath << "\n";
        exit(1);
    }
}

uint64_t Abstractions::NSF::dataLength(int streamId, int entryId) {
    std::cout << "NOT IMPLEMENTED\n";
    exit(1);
}

char* Abstractions::NSF::rawData(int streamId, int entryId) {
    std::cout << "NOT IMPLEMENTED\n";
    exit(1);
}

void Abstractions::NSF::toFASTA(std::ofstream* outputStream) {
    std::cout << "NOT IMPLEMENTED\n";
    exit(1);
}
