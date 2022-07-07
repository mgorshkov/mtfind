//
// Created by Mikhail Gorshkov on 06.07.2022.
//

#include "StatCollector.hpp"

void StatCollector::registerMatch(long lineNumber, int pos, const std::string &match) {
    std::unique_lock<std::mutex> lock{m_mutex};
    m_matches[lineNumber] = {pos, match};
}

std::ostream &operator<<(std::ostream &os, StatCollector &collector) {
    std::unique_lock<std::mutex> lock{collector.m_mutex};
    os << collector.m_matches.size() << std::endl;
    for (const auto &element: collector.m_matches) {
        os << element.first << " " << element.second.pos << " " << element.second.match << std::endl;
    }
    return os;
}