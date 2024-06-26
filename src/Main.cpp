/*

Copyright (c) Alex Baldwin 2022.

This file is part of Kontig.

Kontig is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License Version 3 as published by the Free Software Foundation.

Kontig is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Kontig. If not, see <https://www.gnu.org/licenses/>. 

*/

#include <iostream>
#include <fstream>
#include <memory>
#include <vector>

#include <stdlib.h>
#include <cstring>

#include <stdio.h>

#include "Translators/CLI.h"
#include "Generators/CLI.h"
#include "Stats/CLI.h"

int main(int argc, char** argv) {
    uint8_t kontig_mode = 0;
    
    for (int i = 1; i < 2 && i < argc; ++i) {
        char* arg = argv[i];
        if (strcmp(arg,"help") == 0 || strcmp(arg,"-h") == 0 || strcmp(arg,"--help") == 0) {
            kontig_mode = 0;
            continue;
        }
        if (strcmp(arg,"convert") == 0) {
            kontig_mode = 1;
            continue;
        }
        if (strcmp(arg,"conv") == 0) {
            kontig_mode = 1;
            continue;
        }
        if (strcmp(arg,"translate") == 0) {
            kontig_mode = 1;
            continue;
        }
        if (strcmp(arg,"trans") == 0) {
            kontig_mode = 1;
            continue;
        }
        if (strcmp(arg,"generate") == 0) {
            kontig_mode = 2;
            continue;
        }
        if (strcmp(arg,"gen") == 0) {
            kontig_mode = 2;
            continue;
        }
        if (strcmp(arg,"assemble") == 0) {
            kontig_mode = 3;
            continue;
        }
        if (strcmp(arg,"split") == 0) {
            kontig_mode = 5;
            continue;
        }
        if (strcmp(arg,"join") == 0) {
            kontig_mode = 6;
            continue;
        }
        if (strcmp(arg,"work") == 0) {
            kontig_mode = 7;
            continue;
        }
        if (strcmp(arg,"worker") == 0) {
            kontig_mode = 7;
            continue;
        }
        if (strcmp(arg,"master") == 0) {
            kontig_mode = 8;
            continue;
        }
        if (strcmp(arg,"controller") == 0) {
            kontig_mode = 8;
            continue;
        }
        if (strcmp(arg,"control") == 0) {
            kontig_mode = 8;
            continue;
        }
        if (strcmp(arg,"stat") == 0) {
            kontig_mode = 16;
            continue;
        }
        if (strcmp(arg,"stats") == 0) {
            kontig_mode = 16;
            continue;
        }
        if (strcmp(arg,"statistics") == 0) {
            kontig_mode = 16;
            continue;
        }
        std::cerr << "Unknown command " << arg << "\n\n";
        kontig_mode = 0;
        break;
    }

    switch (kontig_mode) {
        case 1:
            new Translators::CLI(argc, argv);
            break;
        case 2:
            new Generators::CLI(argc, argv);
            break;
        case 3:
            std::cout << "Assemble [TODO]\n";
            break;
        case 5:
            std::cout << "Split [TODO]\n";
            break;
        case 6:
            std::cout << "Join [TODO]\n";
            break;
        case 7:
            std::cout << "Worker [TODO]\n";
            break;
        case 8:
            std::cout << "Controller [TODO]\n";
            break;
        case 16:
            new Stats::CLI(argc, argv);
            break;
        default:
            std::cout << "Usage: kontig <mode> [options and flags]\n";
            std::cout << "Hybrid DNA read assembler v0.0\n";
            std::cout << "\n";
            std::cout << "For information about a particular mode\n";
            std::cout << "  kontig <mode> --help\n";
            std::cout << "\n";
            std::cout << "Avaliable modes\n";
            std::cout << "  convert/trans/translate\n";
            std::cout << "  Converts different file types into ones understood by kontig (and other tools) natively\n";
            std::cout << "  stats/statistics\n";
            std::cout << "  Shows relevant statistics for a given input file\n";
            std::cout << "\n";
            std::cout << "Full documentation <https://kontig.alexbaldwin.dev/docs>\n";
            exit(0);
            break;
    }
        
    return 0;
}
