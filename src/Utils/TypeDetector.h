#ifndef UTILS_TYPE_DETECTOR_H
#define UTILS_TYPE_DETECTOR_H

#include "FileType.h"

namespace Utils {
    class TypeDetector {
    public:
        static char* extract_file_extension(char* filename);
        static Utils::FileType file_extension_to_filetype(char* extension);
    };
}

#endif
