add_library (raptorhw-rtlsdr main.cpp   "conversion_table.h" "conversion_table.cpp" "include.h")

# Find RTL-SDR library
add_subdirectory(lib)

# Link our library to the RTL-SDR one
target_link_libraries(raptorhw-rtlsdr PRIVATE rtlsdr_static)