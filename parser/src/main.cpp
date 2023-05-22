#include <thread>
#include <iostream>

#include "bed_parser.hpp"
#include "protobuf_writer.hpp"

using std::thread;

void parse(BedParser* parser) {
    parser->Parse();
}

void write_func(ProtoWriter* writer) {
    writer->Start();
}

int main (int argc, char* argv[]) {
    path dir = argv[2];
    dir += "/";
    SharedBedLine line;
    atomic<bool> finished = false;
    BedParser p (argv[1], &line);
    thread parser(parse, &p);
    ProtoWriter wr(dir, &line, &finished);
    thread writer(write_func, &wr);
    parser.join();
    finished = true;
    writer.join();
    return 0;
}
