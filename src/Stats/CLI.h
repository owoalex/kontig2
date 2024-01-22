#ifndef STATS_CLI_H
#define STATS_CLI_H

#include <vector>
#include "../Utils/FileType.h"

namespace Stats {
    class CLI {
    public:
        CLI(int argc, char** argv);
        static char* extract_file_extension(char* filename);
        static Utils::FileType file_extension_to_filetype(char* extension);
    };
}

#endif
