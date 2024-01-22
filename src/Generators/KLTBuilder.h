#ifndef GENERATORS_KLT_BUILDER_H
#define GENERATORS_KLT_BUILDER_H

#include "../Abstractions/NSF.h"
#include "../Utils/FunctionStatus.h"
#include <fstream>
#include <iostream>

namespace Generators {
    class KLTBuilder {

    public:
        KLTBuilder();
        Utils::FunctionStatus generate(Abstractions::NSF* nsf);
    };
}

#endif

