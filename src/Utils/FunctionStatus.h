#ifndef UTILS_FUNCTION_STATUS_H
#define UTILS_FUNCTION_STATUS_H

namespace Utils {
    enum FunctionStatus {
        Success = 0,
        UnknownFailure,
        InvalidConfiguration,
        MathError,
        InputFormatError,
        InputIOError,
        OutputIOError
    };
}

#endif
