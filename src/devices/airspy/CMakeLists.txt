add_library (raptorhw-airspy main.cpp  "include.h")

# Find AirSpy library
add_subdirectory(lib)

# Link our library to the AirSpy one
target_link_libraries(raptorhw-airspy PRIVATE airspy-static)