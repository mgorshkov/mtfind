#pragma once

#include <iostream>
#include <map>
#include <mutex>
#include <string>

#include "IStatCollector.hpp"

class StatCollector : public IStatCollector {
public:
    void registerMatch(long lineNumber, int pos, const std::string &match) override;

private:
    friend std::ostream &operator<<(std::ostream &ostream, StatCollector &collector);

    struct Match {
        int pos;
        std::string match;
    };
    std::map<long, Match> m_matches;
    std::mutex m_mutex;
};
