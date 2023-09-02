#ifndef ABSTRACTIONS_NSF_H
#define ABSTRACTIONS_NSF_H

#include <fstream>
#include <iostream>

namespace Abstractions {
    class NSF {
    private:
        char* filePath;
        std::fstream* fileStream;
        int streamCount = 0;
        int* streamIndexOffsets;
        uint8_t* streamIndexSizes;
        uint8_t* streamTypes;
        int* streamEntryCounts;
        uint64_t getUInt64AtOffset(int offset);
        uint8_t getUInt8AtOffset(int offset);
        
    public:
        const char MAGIC_NUMBER[4] = {0x62, 0x6e, 0x61, 0x00};
        const int HEADER_SIZE = 64;
        const int STREAM_HEADER_SIZE = 64;
        NSF(char* filePath);
        uint64_t getEntryCount(int streamId);
        uint64_t dataLength(int streamId, int entryId);
        uint64_t dataOffset(int streamId, int entryId);
        char* getData(int streamId, int entryId);
        uint8_t getStreamCount();
        void toFASTA(std::ofstream* outputStream);
    };
}

#endif
