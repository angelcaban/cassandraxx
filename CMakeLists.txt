cmake_minimum_required(VERSION 3.10)
include(ExternalProject)

project(CassandraCxx)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)
set(LIBUV_ROOT_DIR "${PROJECT_SOURCE_DIR}/third_party/install"
    CACHE PATH "libuv install directory")
set(LIBUV_LIBRARY "${PROJECT_SOURCE_DIR}/third_party/install/lib/libuv.a"
    CACHE PATH "libuv library install directory")
set(LIBUV_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/third_party/install/include"
    CACHE PATH "libuv include install directory")
set(CPPDRIVER_ROOT_DIR "${PROJECT_SOURCE_DIR}/third_party/install"
    CACHE PATH "cpp-driver install directory")

include_directories(
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/third_party/install/include"
)

file(GLOB srcs src/*.cpp)
file(GLOB hdrs include/*.hpp)

externalproject_add(libuv
                    PREFIX "${LIBUV_ROOT_DIR}"
                    SOURCE_DIR "${PROJECT_SOURCE_DIR}/third_party/libuv"
                    CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${LIBUV_ROOT_DIR}")
externalproject_add(cpp-driver
                    PREFIX "${CPPDRIVER_ROOT_DIR}"
                    SOURCE_DIR "${PROJECT_SOURCE_DIR}/third_party/cpp-driver"
                    DEPENDS libuv
                    CMAKE_ARGS "-DLIBUV_ROOT_DIR=${LIBUV_ROOT_DIR} -DCASS_BUILD_SHARED=1 -DCMAKE_INSTALL_PREFIX=${CPPDRIVER_ROOT_DIR}")

add_library(CassandraCxx SHARED ${srcs} ${hdrs})
add_dependencies(CassandraCxx cpp-driver libuv)
target_link_libraries(CassandraCxx cassandra)
