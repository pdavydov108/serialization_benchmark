#include <benchmark/benchmark.h>
#include <login_generated.h>
#include <transaction_generated.h>
#include <random>
#include <flatbuffers/flexbuffers.h>

static void BM_login_flat(benchmark::State &state) {
    flatbuffers::FlatBufferBuilder builder{1024};
    while (state.KeepRunning()) {
        auto hostname = builder.CreateString("localhost");
        auto user = builder.CreateString("pablo");
        auto pass = builder.CreateString("secret");
        LoginBuilder lb{builder};
        lb.add_hostname(hostname);
        lb.add_name(user);
        lb.add_objectType(1);
        lb.add_timeout(1024);
        lb.add_password(pass);
        auto login = lb.Finish();
        builder.Finish(login);
        auto buff = builder.GetBufferPointer();
        auto size = builder.GetSize();
        builder.Clear();
    }
}
BENCHMARK(BM_login_flat);

static void BM_login_parse_flat(benchmark::State &state) {
    flatbuffers::FlatBufferBuilder builder{1024};
    while (state.KeepRunning()) {
        auto hostname = builder.CreateString("localhost");
        auto user = builder.CreateString("pablo");
        auto pass = builder.CreateString("secret");
        LoginBuilder lb{builder};
        lb.add_hostname(hostname);
        lb.add_name(user);
        lb.add_objectType(1);
        lb.add_timeout(1024);
        lb.add_password(pass);
        auto login = lb.Finish();
        builder.Finish(login);
        auto buff = builder.GetBufferPointer();
        auto size = builder.GetSize();
        builder.Clear();
        auto lg_read = flatbuffers::GetRoot<Login>(buff);
        benchmark::DoNotOptimize(lg_read);
    }
}
BENCHMARK(BM_login_parse_flat);

static void BM_transaction_flat(benchmark::State &state) {
    flatbuffers::FlatBufferBuilder builder{1024};
    while (state.KeepRunning()) {
        TransactionBuilder lb{builder};
        lb.add_timeout(1);
        auto trans = lb.Finish();
        builder.Finish(trans);
        auto buff = builder.GetBufferPointer();
        auto size = builder.GetSize();
        builder.Clear();
    }
}
BENCHMARK(BM_transaction_flat);

static void BM_login_random_flat(benchmark::State &state) {
    std::random_device seed;
    std::mt19937 gen(seed());
    std::vector<std::string> random_strings;
    std::generate_n(std::back_inserter(random_strings), 1024, [&] { return std::to_string(gen()); });

    std::vector<char> buffer(1024, 0);
    flatbuffers::FlatBufferBuilder builder{4096};
    long i = 0;
    while (state.KeepRunning()) {
        auto hostname = builder.CreateString(random_strings[++i % 1024]);
        auto user = builder.CreateString(random_strings[++i % 1024]);
        auto pass = builder.CreateString(random_strings[++i % 1024]);
        LoginBuilder lb{builder};
        lb.add_hostname(hostname);
        lb.add_name(user);
        lb.add_objectType(1);
        lb.add_timeout(1024);
        lb.add_password(pass);
        auto login = lb.Finish();
        builder.Finish(login);
        auto buff = builder.GetBufferPointer();
        auto size = builder.GetSize();
        builder.Clear();
    }
}
BENCHMARK(BM_login_random_flat);

static void BM_login_flex(benchmark::State &state) {
    flexbuffers::Builder builder{1024};
    while (state.KeepRunning()) {
        builder.String("localhost");
        builder.String("pablo");
        builder.Int(1);
        builder.Int(1024);
        builder.String("secret");
        builder.Finish();
        auto &buff = builder.GetBuffer();
        builder.Clear();
    }
}
BENCHMARK(BM_login_flex);

static void BM_login_random_flex(benchmark::State &state) {
    std::random_device seed;
    std::mt19937 gen(seed());
    std::vector<std::string> random_strings;
    std::generate_n(std::back_inserter(random_strings), 1024, [&] { return std::to_string(gen()); });

    flexbuffers::Builder builder{1024 * 1024};
    long i = 0;
    while (state.KeepRunning()) {
        builder.String(random_strings[++i % 1024]);
        builder.String(random_strings[++i % 1024]);
        builder.Int(1);
        builder.Int(1024);
        builder.String(random_strings[++i % 1024]);
        builder.Finish();
        auto &buff = builder.GetBuffer();
        builder.Clear();
    }
}
BENCHMARK(BM_login_random_flex);

