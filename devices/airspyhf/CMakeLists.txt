add_library (raptorhw-devices-airspyhf STATIC "include/raptorhw_device_airspyhf.h"  "src/module.cpp" "src/internal.h" "src/candidate.cpp" "src/candidate.h" "src/device.cpp" "src/device.h" "src/gain.h" "src/gain.cpp" "src/custom_properties.h" "src/custom_properties.cpp")
add_subdirectory(lib)
target_include_directories(raptorhw-devices-airspyhf PUBLIC  
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(raptorhw-devices-airspyhf raptorhw-common airspyhf-static)