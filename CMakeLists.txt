cmake_minimum_required (VERSION 3.8)
project ("raptorhw")

#############################
# ADD COMMON
#############################
add_subdirectory(common)

#############################
# FIND COMMON COMPONENTS
#############################
if(WIN32)
    find_package(LIBUSB REQUIRED)
    find_package(pthreads REQUIRED)
    include_directories(${LIBUSB_INCLUDE_DIR} ${PThreads4W_INCLUDE_DIR})
else()
    # Find LibUSB
    find_package(PkgConfig)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(PC_LIBUSB libusb-1.0)
    endif(PKG_CONFIG_FOUND)

    FIND_PATH(LIBUSB_INCLUDE_DIR libusb.h
    PATHS ${PC_LIBUSB_INCLUDEDIR} ${PC_LIBUSB_INCLUDE_DIRS})

    FIND_LIBRARY(LIBUSB_LIBRARIES NAMES usb-1.0
    PATHS ${PC_LIBUSB_LIBDIR} ${PC_LIBUSB_LIBRARY_DIRS})

    include(FindPackageHandleStandardArgs)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(LIBUSB DEFAULT_MSG LIBUSB_LIBRARIES LIBUSB_INCLUDE_DIR)

    MARK_AS_ADVANCED(LIBUSB_INCLUDE_DIR LIBUSB_LIBRARIES)

    # Add
    find_package(Threads REQUIRED)
    include_directories(${LIBUSB_INCLUDE_DIR} ${THREADS_PTHREADS_INCLUDE_DIR})
endif()

#############################
# ADD COMPONENTS
#############################

set(RAPTORHW_DEVICE_LIBS "")

# AIRSPY
option(RAPTORHW_DEVICE_AIRSPY "Enable AirSpy support" ON)
if (RAPTORHW_DEVICE_AIRSPY)
add_compile_definitions(RAPTORHW_DEVICE_AIRSPY)
add_subdirectory(devices/airspy)
list(APPEND RAPTORHW_DEVICE_LIBS "raptorhw-devices-airspy")
endif()

# RTLSDR
option(RAPTORHW_DEVICE_RTLSDR "Enable RTL-SDR support" ON)
if (RAPTORHW_DEVICE_RTLSDR)
add_compile_definitions(RAPTORHW_DEVICE_RTLSDR)
add_subdirectory(devices/rtlsdr)
list(APPEND RAPTORHW_DEVICE_LIBS "raptorhw-devices-rtlsdr")
endif()

#############################
# ADD CORE
#############################

add_library (raptorhw SHARED "context.cpp" "context.h" "api.cpp" "modules.cpp" "search.cpp")
target_link_libraries (raptorhw PRIVATE raptorhw-common ${RAPTORHW_DEVICE_LIBS})

target_include_directories(raptorhw PUBLIC  
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
    $<INSTALL_INTERFACE:include>
)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

#############################
# Create installation target
#############################

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