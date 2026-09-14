cmake_minimum_required(VERSION 3.5)
project(chip8)

# Create an option to switch between a system sdl library and a vendored SDL library
option(MYGAME_VENDORED "Use vendored libraries" ON)

if(MYGAME_VENDORED)
    # add vendored sdl3 path to cmake prefix so that find_package works
    set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/vendor/SDL3" ${CMAKE_PREFIX_PATH})
endif()

# 1. Look for a SDL3 package,
# 2. look for the SDL3-shared component, and
# 3. fail if the shared component cannot be found.
find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)

# Create your game executable target as usual
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE src/main.c src/chip8.c src/stack.c)

# Link to the actual SDL3 library.
target_link_libraries(chip8 PRIVATE SDL3::SDL3)

# copy SDL3.dll to bin directory
if(MYGAME_VENDORED)
    add_custom_command(TARGET chip8
        POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${CMAKE_SOURCE_DIR}/vendor/SDL3/x86_64-w64-mingw32/bin/SDL3.dll"
                "$<TARGET_FILE_DIR:chip8>/SDL3.dll"
    )
endif()

# copy roms directory to bin directory
add_custom_command(TARGET chip8
    POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/roms"
            "$<TARGET_FILE_DIR:chip8>/roms"
)
