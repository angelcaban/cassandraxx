cmake_minimum_required(VERSION 3.10)
include(ExternalProject)

project(CassandraCxx
        VERSION 1.0.0.0
        DESCRIPTION "Cassandra C++ Driver Wrapper"
        LANGUAGES CXX)

add_definitions("-std=gnu++17")

find_package(Boost REQUIRED
             COMPONENTS
               date_time
               filesystem
               system)

# set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)
set(THIRD_PARTY_INSTALL "${PROJECT_SOURCE_DIR}/third_party/install"
    CACHE PATH "third-party install directory")

include_directories(
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/third_party/install/include"
    "${Boost_INCLUDE_DIRS}"
)

file(GLOB srcs src/*.cpp)
file(GLOB hdrs include/*.hpp)

link_directories(
    "${THIRD_PARTY_INSTALL}/lib"
    "${THIRD_PARTY_INSTALL}/lib64"
)

externalproject_add(libuv
                    SOURCE_DIR "${PROJECT_SOURCE_DIR}/third_party/libuv"
                    GIT_REPOSITORY "https://github.com/libuv/libuv.git"
                    GIT_TAG "v1.x"
                    CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${THIRD_PARTY_INSTALL}")
externalproject_add(cpp-driver
                    SOURCE_DIR "${PROJECT_SOURCE_DIR}/third_party/cpp-driver"
                    GIT_REPOSITORY "https://github.com/datastax/cpp-driver.git"
                    DEPENDS libuv
                    INSTALL_DIR "${THIRD_PARTY_INSTALL}"
                    CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${THIRD_PARTY_INSTALL}")

add_library(CassandraCxx SHARED ${srcs} ${hdrs})
add_dependencies(CassandraCxx cpp-driver libuv)
target_link_libraries(CassandraCxx cassandra)
