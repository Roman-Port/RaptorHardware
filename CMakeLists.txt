cmake_minimum_required (VERSION 3.8)
project ("raptorhw")

add_library (raptorhw SHARED  "src/defines.h" "include/raptorhw.h" "src/candidate.cpp" "src/instance.cpp" "src/search.cpp" "src/gain.cpp")

add_subdirectory (src/devices/rtlsdr)
add_subdirectory (src/devices/airspy)
target_link_libraries (raptorhw PRIVATE raptorhw-rtlsdr raptorhw-airspy)

target_include_directories(raptorhw PUBLIC  
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
    $<INSTALL_INTERFACE:include>
)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(demo)

# Create installation target

install(TARGETS raptorhw EXPORT raptorhw-targets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)

install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include
  DESTINATION
    include
  COMPONENT
    Devel
)

export(PACKAGE raptorhw)
configure_file(cmake/raptorhw-config.cmake
  "${CMAKE_CURRENT_BINARY_DIR}/raptorhw/raptorhw-config.cmake"
  COPYONLY
)

set(ConfigPackageLocation lib/cmake/raptorhw)
install(EXPORT raptorhw-targets
  FILE
    raptorhw-targets.cmake
  DESTINATION
    ${ConfigPackageLocation}
)

install(
  FILES
    cmake/raptorhw-config.cmake
  DESTINATION
    ${ConfigPackageLocation}
  COMPONENT
    Devel
)