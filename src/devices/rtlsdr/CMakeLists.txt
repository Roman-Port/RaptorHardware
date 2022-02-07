add_library (raptorhw-rtlsdr main.cpp   "conversion_table.h" "conversion_table.cpp" "include.h")

# Find RTL-SDR library
find_package(rtlsdr CONFIG REQUIRED)

# Link our library to the RTL-SDR one
target_link_libraries(raptorhw-rtlsdr PRIVATE rtlsdr::rtlsdr)