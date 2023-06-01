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
        NSF(char* filePath);
        int getStreams();
        int getEntries();
        uint64_t dataLength(int streamId, int entryId);
        char* rawData(int streamId, int entryId);
        void toFASTA(std::ofstream* outputStream);
    };
}

#endif
