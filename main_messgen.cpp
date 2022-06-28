#include <benchmark/benchmark.h>
#include <messgen/Metadata.h>
#include <messages/msgs/protocol/test/messages.h>
#include <vector>
#include <random>

static void BM_login_messg(benchmark::State &state) {
    std::vector<char> buffer(1024, 0);
    while (state.KeepRunning()) {
        messages::msgs::protocol::test::login_flat lf;
        lf.hostname = "localhost";
        lf.object_name = "pablo";
        lf.object_type = 1;
        lf.timeout = 1024;
        lf.password = "secret";
        benchmark::DoNotOptimize(lf.serialize_msg(reinterpret_cast<uint8_t*>(buffer.data())));
    }
}
BENCHMARK(BM_login_messg);

static void BM_login_parse_messg(benchmark::State &state) {
    std::vector<char> buffer(1024, 0);
    std::vector<char> mem(1024, 0);
    std::string name;
    messgen::MemoryAllocator alloc{reinterpret_cast<uint8_t*>(mem.data()), mem.size()};
    while (state.KeepRunning()) {
        messages::msgs::protocol::test::login_flat lf;
        lf.hostname = "localhost";
        lf.object_name = name;
        lf.object_type = 1;
        lf.timeout = 1024;
        lf.password = "secret";
        auto size = lf.serialize_msg(reinterpret_cast<uint8_t*>(buffer.data()));
        messages::msgs::protocol::test::login_flat other;
        other.parse_msg(reinterpret_cast<uint8_t*>(buffer.data()), size, alloc);
    }
}
BENCHMARK(BM_login_parse_messg);

static void BM_transaction_messg(benchmark::State &state) {
    std::vector<char> buffer(1024, 0);
    while (state.KeepRunning()) {
        messages::msgs::protocol::test::transaction tr;
        tr.prio = 1;
        benchmark::DoNotOptimize(tr.serialize_msg(reinterpret_cast<uint8_t*>(buffer.data())));
    }
}
BENCHMARK(BM_transaction_messg);

static void BM_login_random_messg(benchmark::State &state) {
    std::random_device seed;
    std::mt19937 gen(seed());
    std::vector<std::string> random_strings;
    std::generate_n(std::back_inserter(random_strings), 1024, [&] { return std::to_string(gen()); });

    std::vector<char> buffer(1024, 0);
    long i = 0;
    while (state.KeepRunning()) {
        messages::msgs::protocol::test::login_flat lf;
        lf.hostname = random_strings[++i % 1024];
        lf.object_name = random_strings[++i % 1024];
        lf.object_type = 1;
        lf.timeout = 1024;
        lf.password = random_strings[++i % 1024];
        benchmark::DoNotOptimize(lf.serialize_msg(reinterpret_cast<uint8_t*>(buffer.data())));
    }
}
BENCHMARK(BM_login_random_messg);

