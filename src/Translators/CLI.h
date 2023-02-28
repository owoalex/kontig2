#ifndef TRANSLATORS_CLI_H
#define TRANSLATORS_CLI_H

#include <vector>
#include "FileType.h"

namespace Translators {
    class CLI {
    public:
        CLI(int argc, char** argv);
        static char* extract_file_extension(char* filename);
        static Translators::FileType file_extension_to_filetype(char* extension);
    };
}

#endif
