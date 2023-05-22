#include "bed_parser.hpp"
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

using boost::filesystem::path;
using boost::regex;
using boost::regex_search;

BedParser::BedParser(const string& pathString, SharedBedLine* line)
: Line(line)
{
   File.rdbuf()->pubsetbuf(buffer, buff_size);
   File.open(path(pathString));
}

BedParser::~BedParser() {
    File.close();
}

BedLine* BedParser::ParseLine() {
    BedLine* res = new BedLine;
    BedLine& result = *res;
    string chrom;
    File >> chrom;
    result.Chrom = MAKE_ENUM(chrom);
    File >> result.Left;
    File >> result.Right;

    string s;
    File >> s;
    boost::match_results<string::const_iterator> what;
    regex e("Name=(.*?)%");
    string::const_iterator start, end;
    start = s.cbegin();
    end = s.cend();
    boost::match_flag_type flags = boost::match_default;
    if (regex_search(start, end, what, e, flags)) {
        result.Name = string(what[0].first+5, what[0].second-1);
    } else {
        result.Name = "error";
    }
    File >> result.Peak;
    constexpr auto max_size = std::numeric_limits<std::streamsize>::max();
    File.ignore(max_size, '\n');
    return res;
}


void BedParser::Parse() {
    while (File) {
        Line->Produce(std::move(ParseLine()));
    }
}

