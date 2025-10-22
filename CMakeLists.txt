cmake_minimum_required(VERSION 3.16)
project(CS512-Funtime VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    # Windows - find Raylib manually
    find_path(RAYLIB_INCLUDE_DIR
        NAMES raylib.h
        PATHS 
            "$ENV{RAYLIB_HOME}/include"
            "$ENV{RAYLIB_PATH}/include"
            "C:/raylib/include"
    )
    find_library(RAYLIB_LIBRARY
        NAMES raylib libraylib
        PATHS
            "$ENV{RAYLIB_HOME}/lib"
            "$ENV{RAYLIB_PATH}/lib"
            "C:/raylib/lib"
    )
    if(NOT RAYLIB_INCLUDE_DIR OR NOT RAYLIB_LIBRARY)
        message(FATAL_ERROR "Raylib not found on Windows. Set RAYLIB_HOME or RAYLIB_PATH.")
    endif()
    message(STATUS "Found Raylib on Windows: ${RAYLIB_INCLUDE_DIR}")
    set(RAYLIB_LIBRARIES ${RAYLIB_LIBRARY})
    set(RAYLIB_INCLUDE_DIRS ${RAYLIB_INCLUDE_DIR})
    set(RAYLIB_LIBRARY_DIRS "")           # not used on this path
    set(RAYLIB_CFLAGS_OTHER "")
    set(RAYLIB_LDFLAGS_OTHER "")
else()
    # Linux/macOS - use pkg-config
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(RAYLIB REQUIRED raylib)
    # pkg_check_modules defines:
    #   RAYLIB_LIBRARIES         (e.g., raylib)
    #   RAYLIB_INCLUDE_DIRS      (e.g., /opt/homebrew/include)
    #   RAYLIB_LIBRARY_DIRS      (e.g., /opt/homebrew/lib)
    #   RAYLIB_CFLAGS_OTHER
    #   RAYLIB_LDFLAGS_OTHER     (framework flags on macOS)
endif()

# Directories
include_directories(src)
include_directories(src/chat)
include_directories(src/tilemap)

# -----------------------
# Main Game Executable
# -----------------------
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/character.cpp
    src/test_object.cpp
    src/chat/chat.cpp
    src/tilemap/tilemap.cpp
    src/Collectible.cpp
    # (HUD is header-only)
)

# Include dirs, lib dirs, compile & link flags
target_include_directories(${PROJECT_NAME} PRIVATE ${RAYLIB_INCLUDE_DIRS})
target_link_directories(${PROJECT_NAME} PRIVATE ${RAYLIB_LIBRARY_DIRS})
target_compile_options(${PROJECT_NAME} PRIVATE ${RAYLIB_CFLAGS_OTHER})
target_link_options(${PROJECT_NAME} PRIVATE ${RAYLIB_LDFLAGS_OTHER})
target_link_libraries(${PROJECT_NAME} ${RAYLIB_LIBRARIES})


if(WIN32)
    target_link_libraries(${PROJECT_NAME}
        opengl32
        gdi32
        winmm
    )
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/assets"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
)

# -----------------------
# Deepthi's Sound Test
# -----------------------
add_executable(sound_test
    src/sound_test.cpp
)
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_include_directories(sound_test PRIVATE ${RAYLIB_INCLUDE_DIRS})
target_link_directories(sound_test PRIVATE ${RAYLIB_LIBRARY_DIRS})
target_compile_options(sound_test PRIVATE ${RAYLIB_CFLAGS_OTHER})
target_link_options(sound_test PRIVATE ${RAYLIB_LDFLAGS_OTHER})
target_link_libraries(sound_test ${RAYLIB_LIBRARIES})
target_include_directories(sound_test PRIVATE include)


if(WIN32)
    target_link_libraries(sound_test
        opengl32
        gdi32
        winmm
    )
endif()

set_target_properties(sound_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

add_custom_command(TARGET sound_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/assets"
            "$<TARGET_FILE_DIR:sound_test>/assets"
)
