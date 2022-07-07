//
// Created by Mikhail Gorshkov on 06.07.2022.
//

#include "Matcher.hpp"

#include <utility>

Matcher::Matcher(IStatCollector *statCollector, Mask mask)
    : m_statCollector{statCollector}, m_mask{std::move(mask)} {
}

void Matcher::match(BufferInfo &&bufferInfo) {
    auto maskSize = m_mask.size();
    auto lineNumber = bufferInfo.startingLineNumber;
    if (bufferInfo.size < maskSize) {
        return;
    }
    int pos = 1;
    bool skipRestOfLine = false;
    for (std::size_t offset = 0; offset < bufferInfo.size - maskSize; ++offset) {
        std::string match;
        std::size_t i = 0;
        for (; i < maskSize; ++i) {
            auto symbol = bufferInfo.buffer.get()[offset + i];
            if (symbol == '\n' || symbol == '\r') {
                ++lineNumber;
                pos = 0;
                offset += i;
                skipRestOfLine = false;
            } else if (!skipRestOfLine && (m_mask[i] == '?' || m_mask[i] == symbol)) {
                match += symbol;
                continue;
            }

            break;
        }
        if (i == maskSize) {
            m_statCollector->registerMatch(lineNumber, pos, match);
            skipRestOfLine = true;
        }
        ++pos;
    }
}
