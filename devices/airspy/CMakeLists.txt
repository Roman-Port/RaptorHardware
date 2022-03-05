add_library (raptorhw-devices-airspy STATIC "include/raptorhw_device_airspy.h"  "src/main.cpp" "src/gain_macros.h")
add_subdirectory(lib)
target_include_directories(raptorhw-devices-airspy PUBLIC  
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(raptorhw-devices-airspy raptorhw-common airspy-static)