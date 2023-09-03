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
        constexpr static const char MAGIC_NUMBER[] = {0x6e, 0x73, 0x66, 0x2f, 0x00, 0x00, 0x00, 0x00};
        //                                        n     s     f     /    uint  uint  n/a   n/a
        //                                        ^ File type ident ^
        //                                                                ^ Major revision
        //                                                                      ^ Minor revision
        //                                                                            ^ RESERVED
        //                                                                                  ^ RESERVED
        static const uint8_t ROOT_HEADER_SIZE = 64;
        static const uint8_t STREAM_HEADER_SIZE = 64;
        static const uint8_t NUCLEOTIDE_INDEX_ENTRY_SIZE = 32;
        static const uint8_t QUALITY_INDEX_ENTRY_SIZE = 32;
        static const uint8_t TAG_INDEX_ENTRY_SIZE = 32;
        
        static const uint8_t NUCLEOTIDE_STREAM_IDENTIFIER = 8;
        static const uint8_t QUALITY_STREAM_IDENTIFIER = 7;
        static const uint8_t TAG_STREAM_IDENTIFIER = 6;
        
        NSF(char* filePath);
        uint64_t getEntryCount(int streamId);
        char nsfToFastaChar(unsigned char c);
        int getFirstStreamOfType(uint8_t type);
        uint64_t dataLength(int streamId, int entryId);
        uint64_t dataOffset(int streamId, int entryId);
        char* getData(int streamId, int entryId);
        uint8_t getStreamCount();
        void toFASTA(std::ofstream* outputStream);
        void toFASTQ(std::ofstream* outputStream);
    };
}

#endif
