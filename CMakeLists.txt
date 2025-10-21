cmake_minimum_required(VERSION 3.16)
project(CS512-Funtime VERSION 1.0.0)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Raylib package
find_package(PkgConfig REQUIRED)
pkg_check_modules(RAYLIB REQUIRED raylib)

# Directories
include_directories(src)
include_directories(src/chat)
include_directories(src/tilemap)

# Exes
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/character.cpp
    src/character.hpp
    src/test_object.cpp
    src/test_object.hpp
    src/game_object.hpp
    src/chat/chat.cpp      
    src/chat/chat.hpp
    src/tilemap/tilemap.cpp 
    src/tilemap/tilemap.hpp
)

#Raylib
target_link_libraries(${PROJECT_NAME} ${RAYLIB_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${RAYLIB_INCLUDE_DIRS})
target_compile_options(${PROJECT_NAME} PRIVATE ${RAYLIB_CFLAGS_OTHER})

# Output, file is bin/CS512-Funtime
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)