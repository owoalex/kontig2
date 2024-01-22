#include <stdint.h>
#include <stdlib.h>
#include <bit>
#include <cstring>
#include <stdio.h>
#include "KLTBuilder.h"
#include "../Abstractions/NSF.h"
#include "../Utils/FunctionStatus.h"
#include <vector>

Generators::KLTBuilder::KLTBuilder() {
}

Utils::FunctionStatus Generators::KLTBuilder::generate(Abstractions::NSF* nsf) {
    return Utils::FunctionStatus::Success;
}
