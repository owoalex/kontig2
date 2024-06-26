#ifndef TRANSLATORS_FASTQ_READER_H
#define TRANSLATORS_FASTQ_READER_H

#include <fstream>
#include <iostream>

namespace Translators {
    class FASTQReader {
    private:
        std::ifstream* inputStream;
        
    public:
        FASTQReader(std::ifstream* inputStream);
        void toNSF(std::ofstream* outputStream);
    };
}

#endif
