#include "chromosomes.hpp"
#include <unordered_map>
#include <map>
#include <memory>
#include <vector>
#include <boost/filesystem.hpp>

using boost::filesystem::path;
using std::unordered_map;
using std::map;

using TChromoMap = unordered_map<Chr, map<uint32_t, uint32_t>>;

using TCustomMap = unordered_map<string, //name
                            TChromoMap>;

class ProtoWriter {
public:
    ProtoWriter(path dir, SharedBedLine* line, atomic<bool>* finished);

    ~ProtoWriter();

    void Start();


private:
    void Dump();

    void Write(BedLine* bedLine);

    SharedBedLine* Line;


    const path Dir;
    atomic<bool>* Finished;
    TCustomMap Values;
};
