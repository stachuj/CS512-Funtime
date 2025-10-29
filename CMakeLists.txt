cmake_minimum_required(VERSION 3.11)

get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies
set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET)

include(FetchContent)

if (NOT raylib_FOUND)
  # set options before fetching/building raylib
  set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if(NOT raylib_POPULATED)
    set(FETCHCONTENT_QUIET NO)
    FetchContent_MakeAvailable(raylib)
  endif()
endif()

# raylib-cpp (wrapper)
FetchContent_Declare(
  raylib-cpp
  GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
  GIT_TAG        master
)
FetchContent_GetProperties(raylib-cpp)
if(NOT raylib-cpp_POPULATED)
  set(FETCHCONTENT_QUIET NO)
  FetchContent_MakeAvailable(raylib-cpp)
endif()

# Sources & includes
file(GLOB_RECURSE project_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE project_HEADERS CONFIGURE_DEPENDS "src/*.hpp")

set(project_INCLUDE_DIRS "include")
foreach(_headerFile ${project_HEADERS})
  get_filename_component(_dir ${_headerFile} PATH)
  list (APPEND project_INCLUDE_DIRS ${_dir})
endforeach()

add_executable(${PROJECT_NAME} ${project_SOURCES})

# Link raylib (the imported/created target) — use raylib::raylib if that exists
if (TARGET raylib)
  target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
elseif (TARGET raylib::raylib)
  target_link_libraries(${PROJECT_NAME} PRIVATE raylib::raylib)
else()
  # fallback: try linking plain "raylib" (may work when find_package produced variables)
  target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
endif()

# Platform-specific libraries
if(WIN32)
  target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32)
endif()

# Include dirs: raylib-cpp include, project include dirs
if (DEFINED raylib-cpp_SOURCE_DIR)
  target_include_directories(${PROJECT_NAME} PRIVATE ${raylib-cpp_SOURCE_DIR}/include)
endif()
target_include_directories(${PROJECT_NAME} PRIVATE ${project_INCLUDE_DIRS})

# Web/Emscripten: prefer checking EMSCRIPTEN flag
if (EMSCRIPTEN)
  set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")
endif()

# macOS frameworks
if (APPLE)
  target_link_libraries(${PROJECT_NAME} PRIVATE "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
endif()
