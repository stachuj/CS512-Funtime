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
        message("install raylib")
    endif()
    
    message(STATUS "Found Raylib on Windows: ${RAYLIB_INCLUDE_DIR}")
    set(RAYLIB_LIBRARIES ${RAYLIB_LIBRARY})
    set(RAYLIB_INCLUDE_DIRS ${RAYLIB_INCLUDE_DIR})
else()
    # Linux/macOS - use pkg-config
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(RAYLIB REQUIRED raylib)
    # pkg_check_modules defines:
    # RAYLIB_LIBRARIES, RAYLIB_INCLUDE_DIRS, RAYLIB_CFLAGS_OTHER
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
)

# Raylib linking
target_link_libraries(${PROJECT_NAME} ${RAYLIB_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${RAYLIB_INCLUDE_DIRS})

# Windows-specific libraries
if(WIN32)
    target_link_libraries(${PROJECT_NAME} 
        opengl32 
        gdi32 
        winmm
    )
endif()

# Linux/macOS compile options
if(NOT WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE ${RAYLIB_CFLAGS_OTHER})
endif()

# Output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Copy assets so sprites work
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${
