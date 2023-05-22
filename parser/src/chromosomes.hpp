#pragma once

#include <string>
#include <cstdint>
#include <atomic>
#include <map>

using std::string;
using std::map;
using std::atomic;

enum class Chr {
    #include "tmp/enum.inc"
    other,
};

static const map<string, Chr> chromosomes = {
    #include "tmp/enum_map.inc"
};

static const map<Chr, string> chromosomes_rev = {
    #include "tmp/enum_map_reversed.inc"
};

static Chr MAKE_ENUM(const string& s) {
    if (auto it = chromosomes.find(s); it != chromosomes.end()) {
        return it->second;
    } else {
        return Chr::other;
    }
}

static string MAKE_NAME(const Chr& c) {
    if (auto it = chromosomes_rev.find(c); it != chromosomes_rev.end()) {
        return it->second;
    } else {
        return "other";
    }
}

struct BedLine {
    Chr Chrom;
    uint32_t Left;
    uint32_t Right;
    uint32_t Peak;
    string Name;
};

class SharedBedLine {
public:
    SharedBedLine() = default;
    void Produce(BedLine* line) {
        BedLine* expected = nullptr;
        while (true) {
            if (this->Line.compare_exchange_weak(expected, std::move(line))) {
                break;
            }
            expected = nullptr;
        }
    }

    BedLine* Consume() {
        return this->Line.exchange(nullptr);
    }
private:
    atomic<BedLine*> Line = nullptr;
};
