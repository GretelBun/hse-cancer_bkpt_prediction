#pragma once

#include "chromosomes.hpp"

#include <boost/filesystem/fstream.hpp>

using boost::filesystem::ifstream;
using std::string;

class BedParser final {
public:
    BedParser(const string& pathString, SharedBedLine* line);
    ~BedParser();

    void Parse();

private:
    BedLine* ParseLine();
    static constexpr size_t buff_size = 1024 * 1024;
    char buffer [buff_size]; //10M
    SharedBedLine* Line;
    ifstream File;
};
