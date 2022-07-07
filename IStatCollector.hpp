#pragma once

#include <string>

class IStatCollector {
public:
    virtual ~IStatCollector() = default;
    virtual void registerMatch(long lineNumber, int pos, const std::string &match) = 0;
};
