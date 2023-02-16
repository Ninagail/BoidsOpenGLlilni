# ---Choose CMake version---
cmake_minimum_required(VERSION 3.20)

# ---Choose project name---
project(OpenGL-TPs-Template)

# ---Add p6 library---
add_subdirectory(lib/p6)

# ---Create a target for each TP---
function(setup_tp TP_NUMBER)
    set(TARGET_NAME ${TP_NUMBER}_exe)
    add_executable(${TARGET_NAME})

    # ---Choose C++ version---
    target_compile_features(${TARGET_NAME} PRIVATE cxx_std_20)

    # ---Choose warning level---
    if(MSVC)
        target_compile_options(${TARGET_NAME} PRIVATE /W4)
    else()
        target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic -pedantic-errors)
    endif()

    # ---Declare source files---
    file(GLOB_RECURSE TP_FILES CONFIGURE_DEPENDS ${TP_NUMBER}/*)
    file(GLOB_RECURSE COMMON_FILES CONFIGURE_DEPENDS src-common/*)
    target_sources(${TARGET_NAME} PRIVATE ${TP_FILES} ${COMMON_FILES})
    target_include_directories(${TARGET_NAME} PRIVATE ${TP_FILES} src-common)

    # ---Link p6 library---
    target_link_libraries(${TARGET_NAME} PRIVATE p6::p6)

    # ---Copy the assets and the shaders to the output folder (where the executable is created)---
    p6_copy_folder(${TARGET_NAME} assets)
    p6_copy_folder(${TARGET_NAME} ${TP_NUMBER}/shaders)
endfunction(setup_tp)

setup_tp(TP1)
setup_tp(TP2)
setup_tp(TP3)
setup_tp(TP4)
setup_tp(TP5)
setup_tp(TP6)
setup_tp(TP7)
setup_tp(TP8)