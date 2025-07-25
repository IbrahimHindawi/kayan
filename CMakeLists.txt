cmake_minimum_required(VERSION 3.10)

project(kayan VERSION 1.0 LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

if(CMAKE_C_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(your_target_name PRIVATE -Wno-missing-braces)
endif()

include_directories(extern/haikal/src/meta_arena/gen)
include_directories(extern/cglm/include)
add_executable(${PROJECT_NAME}
    src/main.c
    src/fileops.c
)
add_subdirectory(extern) # in case of modules...

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
endif()

# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resource ${CMAKE_CURRENT_BINARY_DIR}/resource)
