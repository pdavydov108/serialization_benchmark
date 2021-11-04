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
find_package(flatc)
find_package(benchmark)
find_program(FLATC flatc)

capnp_generate_cpp(CAPNP_SRCS CAPNP_HDRS "messages/capnproto/login.capnp;messages/capnproto/transaction.capnp")

set(FLATBUFFERS_FLATC_EXECUTABLE ${FLATC})
build_flatbuffers("messages/flatbuffers/login.fbs" "" "buffers_flat" "" ${CMAKE_CURRENT_BINARY_DIR} "" "")

add_executable(main_capnp main_capnp.cpp ${CAPNP_SRCS})
target_link_libraries(main_capnp ${benchmark_LIBS} ${CapnProto_LIBS})
message(${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(main_capnp PRIVATE ${CMAKE_CURRENT_BINARY_DIR} ${CAPNP_INCLUDE_DIRECTORY} ${benchmark_INCLUDE_DIRS})

add_executable(main_flat main_flat.cpp ${FLATBUFFERS_SRCS})
target_link_libraries(main_flat)
target_include_directories(main_flat PRIVATE ${CMAKE_CURRENT_BINARY_DIR} ${FLATBUFFERS_GENERATE_HEADERS} CONAN_PKG::benchmark CONAN_PKG::flatc)
add_dependencies(main_flat buffers_flat)
