cmake_minimum_required(VERSION 3.7.2)
project(City CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (MSVC)
    option(SFML_STATIC_LIBRARIES "Build with SFML static libraries" ON)

    option(CITY_WINDOWS_NO_CONSOLE "Configure build to not see a CMD console when launching game." OFF)
    if(CITY_WINDOWS_NO_CONSOLE)
        set(WIN32 WIN32) # add this flag to add_executable to remove console
    endif()
endif()

find_package(SFGUI REQUIRED)
find_package(yaml-cpp REQUIRED)

file(GLOB_RECURSE CPPS src/*.cpp)
add_executable(City ${WIN32} ${CPPS})

target_include_directories(City PRIVATE includes)
target_link_libraries(City PRIVATE SFGUI::SFGUI yaml-cpp)

if (NOT MSVC)
    target_link_libraries(City PRIVATE stdc++fs)
else()
    # Remove the console on windows
    target_link_libraries(City PRIVATE sfml-main)
endif()