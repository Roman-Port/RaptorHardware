add_library (raptorhw-common STATIC "include/raptorhw_device.h" "src/device.cpp" "include/raptorhw_candidate.h" "include/raptorhw_gain.h" "src/gain.cpp" "include/raptorhw_module.h" "include/raptorhw_util.h" "src/util.cpp" "src/candidate.cpp")
target_include_directories(raptorhw-common PUBLIC  
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
    $<INSTALL_INTERFACE:include>
)