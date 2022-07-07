#pragma once

#include "Using.hpp"
#include <cstddef>

class IMatcher {
public:
    virtual ~IMatcher() = default;

    struct BufferInfo {
        Buffer buffer;
        std::size_t size;
        Buffer bufferExt;
        std::size_t sizeExt;
        long startingLineNumber;
    };

    virtual void match(BufferInfo &&) = 0;
};