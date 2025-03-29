cmake_minimum_required(VERSION 3.29)
project(nng_package_dll)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_definitions(-D_NNG_EXPORTS)
add_library(packaged_nng SHARED nng_package.cpp)
set_target_properties(packaged_nng PROPERTIES
        LIBRARY_OUTPUT_NAME "packaged_nng"
)
target_link_libraries(packaged_nng "${CMAKE_SOURCE_DIR}/library/nng/nng.lib")

