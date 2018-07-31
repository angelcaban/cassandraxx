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
find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)

# set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)
set(THIRD_PARTY_INSTALL "${PROJECT_SOURCE_DIR}/third_party/install"
    CACHE PATH "third-party install directory")

enable_testing()
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
externalproject_add(gtest-lib
                    SOURCE_DIR "${PROJECT_SOURCE_DIR}/third_party/gtest"
                    GIT_REPOSITORY "https://github.com/google/googletest.git"
                    INSTALL_DIR "${THIRD_PARTY_INSTALL}"
                    CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${THIRD_PARTY_INSTALL}")
externalproject_add(ccm-cli
                    SOURCE_DIR "${PROJECT_SOURCE_DIR}/third_party/ccm"
                    GIT_REPOSITORY "https://github.com/riptano/ccm.git"
                    CONFIGURE_COMMAND cd ${PROJECT_SOURCE_DIR}/third_party/ccm;
                                      ./setup.py build
                    BUILD_COMMAND     cd ${PROJECT_SOURCE_DIR}/third_party/ccm;
                                      ./setup.py build
                    INSTALL_COMMAND   cd ${PROJECT_SOURCE_DIR}/third_party/ccm;
                                      ./setup.py install)

add_library(CassandraCxx SHARED ${srcs} ${hdrs})
add_dependencies(CassandraCxx cpp-driver libuv ccm-cli)
target_link_libraries(CassandraCxx cassandra)

file(GLOB libraries ${THIRD_PARTY_INSTALL}/lib/*)
file(COPY ${libraries} DESTINATION ${PROJECT_BINARY_DIR})

add_subdirectory(test)
