#include <benchmark/benchmark.h>
#include <iostream>
#include <messages/capnproto/login.capnp.h>
#include <messages/capnproto/transaction.capnp.h>
#include <capnp/message.h>
#include <capnp/serialize-packed.h>
#include <capnp/serialize.h>

static void BM_login(benchmark::State &state) {
    while (state.KeepRunning()) {
        capnp::MallocMessageBuilder message;
        auto login = message.initRoot<Login>();
        login.setHostname("localhost");
        login.setName("pablo");
        login.setObjectType(1);
        login.setPassword("secret");
        login.setTimeout(1000);
        auto arr = capnp::messageToFlatArray(message);
    }
}
BENCHMARK(BM_login);

static void BM_transaction(benchmark::State &state) {
    while (state.KeepRunning()) {
        capnp::MallocMessageBuilder message;
        auto login = message.initRoot<Transaction>();
        login.setPriority(1);
        auto arr = capnp::messageToFlatArray(message);
    }
}
BENCHMARK(BM_transaction);
