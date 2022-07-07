#pragma once

#include "IMatcher.hpp"
#include "IStatCollector.hpp"
#include "Using.hpp"

class Matcher : public IMatcher {
public:
    Matcher(IStatCollector *statCollector, Mask mask);

    void match(BufferInfo &&bufferInfo) override;

private:
    IStatCollector *m_statCollector;
    Mask m_mask;
};
