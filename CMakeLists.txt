# ---Choose CMake version---
cmake_minimum_required(VERSION 3.20)

# ---Choose project name---
project(OpenGL-TPs-Template)

# ---Add p6 library---
include(FetchContent)
set(P6_RAW_OPENGL_MODE ON CACHE BOOL "")
FetchContent_Declare(
    p6
    GIT_REPOSITORY https://github.com/julesfouchy/p6
    GIT_TAG c962f8c5f3bf8324482facaf2ec178ef4c9aefaa
)
FetchContent_MakeAvailable(p6)

# ---Add tinyobjloader library---
add_subdirectory(tinyobjloader)

# ---Create a target for each exercise---
function(add_exercise EXERCISE_NAME)
    set(TARGET_NAME ${EXERCISE_NAME}_exe)
    add_executable(${TARGET_NAME})

    # ---Choose C++ version---
    target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)

    # ---Choose warning level---
    if(MSVC)
        target_compile_options(${TARGET_NAME} PRIVATE /W4)
    else()
        target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic -pedantic-errors -Wimplicit-fallthrough)
    endif()

    # ---Declare source files---
    file(GLOB_RECURSE EXERCISE_FILES CONFIGURE_DEPENDS ${EXERCISE_NAME}/*)
    file(GLOB_RECURSE COMMON_FILES CONFIGURE_DEPENDS src-common/*)
    target_sources(${TARGET_NAME} PRIVATE ${EXERCISE_FILES} ${COMMON_FILES})
    target_include_directories(${TARGET_NAME} PRIVATE ${EXERCISE_FILES} src-common)

    # ---Link p6 library---
    target_link_libraries(${TARGET_NAME} PRIVATE p6::p6)

     # ---Link tinyobjloader library---
     target_link_libraries(${TARGET_NAME} PRIVATE tinyobjloader)

    # ---Set the output folder---
    set_target_properties(${TARGET_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${EXERCISE_NAME})

    # ---Copy the assets and the shaders to the output folder (where the executable is created)---
    Cool__target_copy_folder(${TARGET_NAME} assets)
    Cool__target_copy_folder(${TARGET_NAME} ${EXERCISE_NAME}/shaders shaders)
endfunction(add_exercise)

add_exercise(src)
