cmake_minimum_required(VERSION 3.5)
project(SerializationBench CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

add_definitions("-std=c++17")

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.16.1/conan.cmake"
        "${CMAKE_BINARY_DIR}/conan.cmake"
        EXPECTED_HASH SHA256=396e16d0f5eabdc6a14afddbcfff62a54a7ee75c6da23f32f7a31bc85db23484
        TLS_VERIFY ON)
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_autodetect(settings)

conan_cmake_install(PATH_OR_REFERENCE ${CMAKE_SOURCE_DIR}
    CMAKE_TARGETS
    BUILD missing
    REMOTE conancenter
    SETTINGS ${settings})

find_package(CapnProto)
find_package(Flatbuffers)
find_package(benchmark)

capnp_generate_cpp(CAPNP_SRCS CAPNP_HDRS "messages/capnproto/login.capnp;messages/capnproto/transaction.capnp")

set(FLATBUFFERS_FLATC_EXECUTABLE ${FLATC})
build_flatbuffers("messages/flatbuffers/login.fbs;messages/flatbuffers/transaction.fbs" "" "buffers_flat" "" ${CMAKE_CURRENT_BINARY_DIR} "" "")

add_executable(main_capnp main_capnp.cpp)
target_link_libraries(main_capnp benchmark::benchmark capnproto::capnproto)

add_executable(main_flat main_flat.cpp ${FLATBUFFERS_SRCS})
target_link_libraries(main_flat flatbuffers::flatbuffers benchmark::benchmark)
add_dependencies(main_flat buffers_flat)

FILE(GLOB CMAKE_CUSTOM_FUNCTION_FILES messgen/integrations/cmake/*.cmake)
FOREACH (CUSTOM_FUNCTION_FILE ${CMAKE_CUSTOM_FUNCTION_FILES})
    INCLUDE(${CUSTOM_FUNCTION_FILE})
ENDFOREACH ()
SET(MESSGEN_DIR ${CMAKE_SOURCE_DIR}/messgen)

MESSGEN_GENERATE_MESSAGES(${CMAKE_SOURCE_DIR} messages/protocol/test ${CMAKE_BINARY_DIR})
message(${MESSGEN_DIR}/port/cpp)

add_executable(main_messgen main_messgen.cpp ${MESSGEN_OUT_FILES})
target_link_libraries(main_messgen benchmark::benchmark)
target_include_directories(main_messgen PRIVATE ${CMAKE_CURRENT_BINARY_DIR} ${MESSGEN_DIR}/port/cpp)

