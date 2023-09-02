#ifndef ABSTRACTIONS_NSF_H
#define ABSTRACTIONS_NSF_H

#include <fstream>
#include <iostream>

namespace Abstractions {
    class NSF {
    private:
        char* filePath;
        std::fstream* fileStream;
        
    public:
        const char MAGIC_NUMBER[4] = {0x62, 0x6e, 0x61, 0x00};
        const int HEADER_SIZE = 64;
        const int INDEX_ENTRY_SIZE = 64;
        NSF(char* filePath);
        uint64_t entryCount(int streamId);
        uint64_t dataLength(int streamId, int entryId);
        uint8_t streamCount();
        char* rawDataPointer(int streamId, int entryId);
        void toFASTA(std::ofstream* outputStream);
    };
}

#endif
