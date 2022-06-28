#include <benchmark/benchmark.h>
#include <login_generated.h>
#include <transaction_generated.h>
#include <iostream>

static void BM_login(benchmark::State &state) {
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
BENCHMARK(BM_login);

static void BM_login_parse(benchmark::State &state) {
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
BENCHMARK(BM_login_parse);

static void BM_transaction(benchmark::State &state) {
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
BENCHMARK(BM_transaction);
