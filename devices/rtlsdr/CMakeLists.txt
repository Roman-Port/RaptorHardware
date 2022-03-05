add_library (raptorhw-devices-rtlsdr STATIC "include/raptorhw_device_rtlsdr.h"  "src/main.cpp" "src/conversion_table.cpp" "src/conversion_table.h" "src/info.h" "src/info.cpp")
add_subdirectory(lib)
target_include_directories(raptorhw-devices-rtlsdr PUBLIC  
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(raptorhw-devices-rtlsdr raptorhw-common rtlsdr_static pthreads)