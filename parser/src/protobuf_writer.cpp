#include "protobuf_writer.hpp"
#include "SparseVector.pb.h"
#include <boost/filesystem/fstream.hpp>

using std::string;
using std::unordered_map;
using boost::filesystem::ofstream;

namespace {

    const static unordered_map<Chr, size_t> CHROM_SIZES = {
        #include "tmp/chrom_sizes.inc"
    };

    static size_t GetChromSize(Chr chr) {
        return CHROM_SIZES.at(chr);
    }

}

ProtoWriter::ProtoWriter(path dir, SharedBedLine* line, atomic<bool>* finished)
    : Dir(dir) 
    , Line(line)
    , Finished(finished)

{
}

ProtoWriter::~ProtoWriter() {
   Dump(); 
}

void ProtoWriter::Write(BedLine* bedLine) {
    if (bedLine->Chrom == Chr::other || bedLine->Name == "error") {
        return;
    }

    auto& vec = Values[bedLine->Name][bedLine->Chrom];
    vec[bedLine->Left] = bedLine->Peak;
    vec[bedLine->Right] = 0;
}

static const std::vector<Chr> CHR_VEC({
    #include "tmp/enum_vec.inc"
});

void ProtoWriter::Start() {
    while (!(*Finished)) {
        BedLine* line = Line->Consume();
        if (line) {
            Write(line);
            delete line;
        }
    }
}

void ProtoWriter::Dump() {
    SparseVector sv;
    auto* map = sv.mutable_chromosomes();
    for (auto& [name, value] : Values) {
        for (auto& chrom : CHR_VEC) {
            auto& vec = value[chrom];
            if (!vec.contains(0)) {
                vec[0] = 0;
            }
            SparseVector_vec spv;
            spv.set_size(GetChromSize(chrom));
            for (auto& [idx, val] : vec) {
                if (vec[idx] != 0) {
                    spv.add_indices(idx);
                    spv.add_data(val);
                }
            }
            vec.clear();
            map->insert({MAKE_NAME(chrom), std::move(spv)});
        }
        path file = Dir;
        file += name;
        file += ".pb";
        ofstream Stream(file);
        sv.SerializeToOstream(&Stream);
        sv.Clear();
        value.clear();
    }
}
