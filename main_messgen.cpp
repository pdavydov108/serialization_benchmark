#include <benchmark/benchmark.h>
#include <messgen/Metadata.h>
#include <messages/msgs/messgen/messages.h>
#include <vector>

static void BM_login(benchmark::State &state) {
    std::vector<char> buffer(1024, 0);
    while (state.KeepRunning()) {
        messages::msgs::messgen::login_flat lf;
        lf.hostname = "localhost";
        lf.object_name = "pablo";
        lf.object_type = 1;
        lf.timeout = 1024;
        lf.password = "secret";
        benchmark::DoNotOptimize(lf.serialize_msg(reinterpret_cast<uint8_t*>(buffer.data())));
    }
}
BENCHMARK(BM_login);

static void BM_login_parse(benchmark::State &state) {
    std::vector<char> buffer(1024, 0);
    std::vector<char> mem(1024, 0);
    std::string name;
    messgen::MemoryAllocator alloc{reinterpret_cast<uint8_t*>(mem.data()), mem.size()};
    while (state.KeepRunning()) {
        messages::msgs::messgen::login_flat lf;
        lf.hostname = "localhost";
        lf.object_name = name;
        lf.object_type = 1;
        lf.timeout = 1024;
        lf.password = "secret";
        auto size = lf.serialize_msg(reinterpret_cast<uint8_t*>(buffer.data()));
        messages::msgs::messgen::login_flat other;
        other.parse_msg(reinterpret_cast<uint8_t*>(buffer.data()), size, alloc);
    }
}
BENCHMARK(BM_login_parse);

static void BM_transaction(benchmark::State &state) {
    std::vector<char> buffer(1024, 0);
    while (state.KeepRunning()) {
        messages::msgs::messgen::transaction tr;
        tr.prio = 1;
        benchmark::DoNotOptimize(tr.serialize_msg(reinterpret_cast<uint8_t*>(buffer.data())));
    }
}
BENCHMARK(BM_transaction);
