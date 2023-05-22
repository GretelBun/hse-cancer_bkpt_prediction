CC = clang++
CFLAGS =-I /opt/homebrew/include -I /opt/boost/include/ -std=c++20 -O3 
LFLAGS =-L /opt/homebrew/lib -L /opt/boost/lib/ -lboost_regex -lprotobuf

PROTO_DIR = proto
SRC_DIR = src
SCRIPTS_DIR = scripts
TMP_DIR = src/tmp

PROTOS = $(wildcard $(PROTO_DIR)/*.proto)
PROTO_GENS = $(patsubst $(PROTO_DIR)/%.proto,$(SRC_DIR)/%.pb.cc,$(PROTOS))
SRCS = $(wildcard $(SRC_DIR)/*.cpp) $(PROTO_GENS)
OBJS = $(SRCS:.cpp=.o)
INCS = $(TMP_DIR)/chrom_sizes.inc $(TMP_DIR)/enum.inc $(TMP_DIR)/enum_map.inc $(TMP_DIR)/enum_map_reversed.inc $(TMP_DIR)/enum_vec.inc

TARGET = bed_parser 

.PHONY: all protos debug tsan asan clean

all: $(INCS) protos $(TARGET)

protos: $(PROTO_GENS)

$(SRC_DIR)/%.pb.cc: $(PROTO_DIR)/%.proto
	protoc -I=$(PROTO_DIR) --cpp_out=$(SRC_DIR) $<

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(TMP_DIR)/chrom_sizes.inc: $(SCRIPTS_DIR)/chromosomes_parser
	$< > $@

$(TMP_DIR)/enum.inc: $(SCRIPTS_DIR)/enum_maker
	$< > $@

$(TMP_DIR)/enum_map.inc: $(SCRIPTS_DIR)/enum_map
	$< > $@

$(TMP_DIR)/enum_map_reversed.inc: $(SCRIPTS_DIR)/enum_map_reversed
	$< > $@

$(TMP_DIR)/enum_vec.inc: $(SCRIPTS_DIR)/enum_vec_maker
	$< > $@

debug: CFLAGS = -std=c++20 -g
debug: $(INCS) protos $(TARGET)

tsan: CFLAGS = -std=c++20 -fsanitize=thread
tsan: $(INCS) protos $(TARGET)

asan: CFLAGS = -std=c++20 -fsanitize=address
asan: $(INCS) protos $(TARGET)

clean:
	rm -f $(SRC_DIR)/*.o $(SRC_DIR)/*.pb.* $(TARGET) $(INCS)

