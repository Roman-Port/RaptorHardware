add_library (raptorhw-airspy main.cpp  "include.h")

# Find AirSpy library
find_package(airspy CONFIG REQUIRED)

# Link our library to the AirSpy one
target_link_libraries(raptorhw-airspy PRIVATE airspy)