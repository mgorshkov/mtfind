#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <thread>
#include <algorithm>
#include <filesystem>
#include <assert.h>

#include "StatCollector.hpp"
#include "ThreadedMatcher.hpp"

static void run(std::istream &infile, uintmax_t filesize, const Mask &mask) {
    auto nThreads = std::thread::hardware_concurrency();
    assert(nThreads != 0);

    StatCollector statCollector;
    {
        std::unique_ptr<IMatcher> matcher = std::make_unique<ThreadedMatcher>(&statCollector, nThreads, mask);
        const std::size_t bufferSize = static_cast<std::size_t>(filesize) / nThreads;
        long lineNumber = 1;
        while (infile) {
            Buffer buffer{new char[bufferSize]};// read by large chunks to improve performance
            infile.read(buffer.get(), bufferSize);
            long lineNumberDiff = std::count(buffer.get(), buffer.get() + bufferSize, '\n');
            auto sizeRead = static_cast<std::size_t>(infile.gcount());
            if (infile) {// extend the buffer by reading more until a line separator or eof
                std::string line;
                std::getline(infile, line);
                sizeRead += line.size();
                ++lineNumberDiff;
                Buffer newBuffer{new char[bufferSize + line.size()]};
                std::memcpy(newBuffer.get(), buffer.get(), bufferSize);
                std::memcpy(newBuffer.get() + bufferSize, line.c_str(), line.size());
                matcher->match({std::move(newBuffer), sizeRead, lineNumber});
            } else {
                matcher->match({std::move(buffer), sizeRead, lineNumber});
            }
            lineNumber += lineNumberDiff;
        }
    }

    std::clog << statCollector;
}

int main(int argc, char **argv) {
    static const char *usage = R"(Multithreaded mask searcher. The mask is a string which may contain a "?" wildcard.
Usage: ./mtfind <infile> <quoted_mask>.
Example: ./mtfind huge.txt "?ad")";

    if (argc == 1) {
        std::cerr << usage << std::endl;
        return 1;
    }
    if (argc < 3) {
        std::cerr << "Incorrect number of arguments" << std::endl;
        std::cerr << usage << std::endl;
        return 1;
    }

    Mask mask{argv[2]};
    if (mask.empty()) {
        std::cerr << "Mask is empty" << std::endl;
        return 1;
    }

    if (mask.find('\n') != -1) {
        std::cerr << "Mask has newline inside" << std::endl;
        return 1;
    }

    std::string infilename{argv[1]};

    std::ifstream infile(infilename);
    if (!infile.is_open()) {
        std::cerr << "Can't open file for reading: " << infilename << std::endl;
        return 1;
    }

    auto filesize = std::filesystem::file_size(infilename);
    run(infile, filesize, mask);
    infile.close();
    return 0;
}
