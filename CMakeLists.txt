cmake_minimum_required(VERSION 3.5)
project(chip8)

# Create an option to switch between a system sdl library and a vendored SDL library
option(MYGAME_VENDORED "Use vendored libraries" ON)

if(MYGAME_VENDORED)
    # This assumes you have added SDL as a submodule in vendored/SDL
    add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
else()
    # 1. Look for a SDL3 package,
    # 2. look for the SDL3-shared component, and
    # 3. fail if the shared component cannot be found.
    find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
endif()

# Create your game executable target as usual
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE main.c stack.c)

# Link to the actual SDL3 library.
target_link_libraries(chip8 PRIVATE SDL3::SDL3)
