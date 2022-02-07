cmake_minimum_required (VERSION 3.8)
project ("raptorhw")

add_library (raptorhw SHARED  "src/defines.h" "include/raptorhw.h" "src/candidate.cpp" "src/instance.cpp" "src/search.cpp" "src/gain.cpp")

add_subdirectory (src/devices/rtlsdr)
add_subdirectory (src/devices/airspy)
target_link_libraries (raptorhw raptorhw-rtlsdr raptorhw-airspy)

target_include_directories(raptorhw PUBLIC  
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
    $<INSTALL_INTERFACE:include>
)

add_subdirectory(demo)