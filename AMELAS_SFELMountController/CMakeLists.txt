# **********************************************************************************************************************
#   AMELAS_SFELMountController: [...].
#
#   Copyright (C) 2023 ROA Team (Royal Institute and Observatory of the Spanish Navy)
#                      < Ángel Vera Herrera, avera@roa.es - angeldelaveracruz@gmail.com >
#                      < Jesús Relinque Madroñal >
#                      AVS AMELAS Team
#                      <>
#
#   This file is part of AMELAS_SFELMountController.
#
#   Licensed under [...]
# **********************************************************************************************************************

# **********************************************************************************************************************
# Updated 03/10/2023
# **********************************************************************************************************************

# **********************************************************************************************************************

# Cmake minimum.
cmake_minimum_required(VERSION 3.21)

# Check version.
if (NOT ${CMAKE_VERSION} VERSION_LESS "3.1.0")
    cmake_policy(SET CMP0054 NEW)
endif()

#Configure the build type.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Configure names.
set(PROJECT "AMELAS_SFELMountController")
set(SHOW_EXTERNALS TRUE)

# Project name.
project(${PROJECT})

# Check the architecture.
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(FATAL_ERROR "Architecture not supported by default.")
endif()

# Set path to additional CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules ${CMAKE_MODULE_PATH})
include(CMakeMacros)

# Set the version of the project.
set(MAJOR_VERSION 1)
set(MINOR_VERSION 1)
set(PATCH_VERSION 1)
set(VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})

# Output directories.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Other variables setting.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(SHOW_EXTERNALS TRUE)
set(FORCE_INSTALL_DIR TRUE)

# External dependecies path.
set(ZMQUTILS_PATH ${CMAKE_SOURCE_DIR}/external/LibZMQUtils)

# Define the install directory for each compiler and operating system.
macro_prepare_install_dir(INSTALL_BIN INSTALL_LIB ${CMAKE_CURRENT_SOURCE_DIR}/../product_v${VERSION})

# Generic Includes.
include_directories(${CMAKE_SOURCE_DIR}/includes)
include_directories(${ZMQUTILS_PATH}/includes)

# **********************************************************************************************************************
# AMELAS INTERFACE LIBRARY
# NOTES: Add more interfaces if neccesary (serial for example).

# Library names.
set(LIB_FULL_NAME "LibAmelasInterface")
set(LIB_DEPS_SET ${LIB_FULL_NAME}_deps)

# Get the header files.
file(GLOB_RECURSE HEADERS
    includes/*.h
    includes/AmelasController/*.h
    includes/AmelasControllerServer/*.h
    includes/AmelasControllerClient/*.h)

# Get the template files.
file(GLOB_RECURSE TEMPLTS
    includes/AmelasController/*.tpp
    includes/AmelasControllerServer/*.tpp
    includes/AmelasControllerClient/*.tpp)

# Get the source files.
file(GLOB_RECURSE SOURCES
    sources/AmelasController/*.cpp
    sources/AmelasControllerServer/*.cpp
    sources/AmelasControllerClient/*.cpp)

# Get the alias files.
macro_get_files_without_extension(ALIAS includes/*)

# Show external libs.
if(SHOW_EXTERNALS)
    file(GLOB_RECURSE EXTERN ${ZMQUTILS_PATH}/includes/*.h)
endif()

# Create a custom group for the templates, aliases and externals.
source_group("Alias Files" FILES ${ALIAS})
source_group("Header Files" FILES ${TEMPLTS})
source_group("External Libs" FILES ${EXTERN})

# Add definitions and the library.
string(TOUPPER ${LIB_FULL_NAME} LIB_FULL_NAME_UPPER)
add_definitions(-D${LIB_FULL_NAME_UPPER}_LIBRARY)
add_library(${LIB_FULL_NAME} SHARED ${SOURCES} ${HEADERS} ${EXTERN} ${TEMPLTS} ${ALIAS})
target_compile_definitions(${LIB_FULL_NAME} PRIVATE -D${LIB_FULL_NAME_UPPER}_LIBRARY)

# Configure the compilers.
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++17 -pthread -O3 -fopenmp")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${LIB_FULL_NAME} PRIVATE /std:c++17 /openmp)
else()
    message(FATAL_ERROR "Compiler not supported by default.")
endif()

# In mingw better do static linking of the libgcc, libwinpthread and libstd.
if (MINGW)
    target_link_libraries(${LIB_FULL_NAME} PRIVATE -static-libgcc -static-libstdc++ -static -lpthread)
endif()

# Set libraries. For win32 only.
if (WIN32)

    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

        target_link_libraries(${LIB_FULL_NAME} PUBLIC
            optimized ${ZMQUTILS_PATH}/lib/mingw-x86_64_release/LibZMQUtils.dll
            debug ${ZMQUTILS_PATH}/lib/mingw-x86_64_debug/LibZMQUtils.dll
            optimized ${ZMQUTILS_PATH}/lib/mingw-x86_64_release/libzmq-v142-mt-4_3_4.dll
            debug ${ZMQUTILS_PATH}/lib/mingw-x86_64_debug/libzmq-v142-mt-4_3_4.dll
            optimized ${ZMQUTILS_PATH}/lib/mingw-x86_64_release/libsodium.dll
            debug ${ZMQUTILS_PATH}/lib/mingw-x86_64_debug/libsodium.dll)

    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

        target_link_libraries(${LIB_FULL_NAME} PUBLIC
            optimized ${ZMQUTILS_PATH}/lib/msvc-x86_64_release/libzmq-v142-mt-4_3_4.lib
            debug ${ZMQUTILS_PATH}/lib/msvc-x86_64_debug/libzmq-v142-mt-4_3_4.lib
            optimized ${ZMQUTILS_PATH}/lib/msvc-x86_64_release/LibZMQUtils.lib
            debug ${ZMQUTILS_PATH}/lib/msvc-x86_64_debug/LibZMQUtils.lib)

    else()
        message(FATAL_ERROR "Compiler not supported by default.")
    endif()

endif()

# ----------------------------------------------------------------------------------------------------------------------

# **********************************************************************************************************************
# PRODUCT APPLICATIONS PART
# NOTES: Add more applications if neccesary.

# ----------------------------------------------------------------------------------------------------------------------
# AMELAS CONTROLLER SERVER

# App config.
set(APP_SERVER "AmelasControllerServerApp")
set(APP_BUILD_FOLDER ${CMAKE_BINARY_DIR}/bin/Applications)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${APP_BUILD_FOLDER})

# Get the header files for the server.
file(GLOB_RECURSE HEADERS
    includes/Applications/AmelasControllerServerApp/*.h)

# Get the source files for server.
file(GLOB_RECURSE SOURCES
    sources/Applications/AmelasControllerServerApp/*.cpp)

# Clean alias files and templates.
set(ALIAS "")
set(TEMPLTS "")

# Set libraries. For win32 only.
if (WIN32)
    set(LIBRARIES ${LIB_FULL_NAME})
endif()

# Setup the server launcher.
macro_setup_deploy_launcher("${APP_SERVER}" "${INSTALL_BIN}" "${LIB_DEPS_SET}")

# In mingw better do static linking of the libgcc, libwinpthread and libstd.
if (MINGW)
    target_link_libraries(${APP_SERVER} PRIVATE -static-libgcc -static-libstdc++ -static -lpthread)
endif()

# ----------------------------------------------------------------------------------------------------------------------

# **********************************************************************************************************************
# SUBPROJECTS PART
# NOTES: Add more examples and test if neccesary.

# Add the diferents subprojects directories.
add_subdirectory(examples)
add_subdirectory(testing)

# **********************************************************************************************************************
# INSTALLATION PART

# Installation process for windows.
if(WIN32)

    # Get the path to the external libs.
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        if(MINGW)
            set(EXTERNAL_LIBS_PATH ${ZMQUTILS_PATH}/lib/mingw-x86_64_debug)
        elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
            set(EXTERNAL_LIBS_PATH ${ZMQUTILS_PATH}/lib/msvc-x86_64_debug)
        else()
            message(FATAL_ERROR "Compiler not supported by default.")
        endif()
    else()
        if(MINGW)
            set(EXTERNAL_LIBS_PATH ${ZMQUTILS_PATH}/lib/mingw-x86_64_release)
        elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
            set(EXTERNAL_LIBS_PATH ${ZMQUTILS_PATH}/lib/msvc-x86_64_release)
        else()
            message(FATAL_ERROR "Compiler not supported by default.")
        endif()
    endif()

    # Install the include files of LibAmelasInterface.
    install(DIRECTORY ${CMAKE_SOURCE_DIR}/includes/
            DESTINATION ${CMAKE_INSTALL_PREFIX}/includes/${LIB_FULL_NAME}
            PATTERN "*.txt" EXCLUDE)

    # Install the include files of LibZMQUtils.
    install(DIRECTORY ${ZMQUTILS_PATH}/includes/LibZMQUtils
            DESTINATION ${CMAKE_INSTALL_PREFIX}/includes/)

    # Install the include files of ZMQ.
    install(DIRECTORY ${ZMQUTILS_PATH}/includes/zmq
            DESTINATION ${CMAKE_INSTALL_PREFIX}/includes/)

    # Install the binaries  of LibZMQUtils to the specified install directory.
    install(TARGETS ${LIB_FULL_NAME}
            LIBRARY DESTINATION ${INSTALL_LIB}
            RUNTIME DESTINATION ${INSTALL_LIB})

    # Install the binaries of the external libs to the specified install directory.
    install(DIRECTORY ${EXTERNAL_LIBS_PATH}/
            DESTINATION ${INSTALL_LIB}
            FILES_MATCHING PATTERN "*.dll")



    # Install the lib files for msvc.
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

        # External libs
        install(DIRECTORY ${EXTERNAL_LIBS_PATH}/
                DESTINATION ${INSTALL_LIB}
                FILES_MATCHING PATTERN "*.lib")

        # Install the LibAmelasInterface.lib file to the specified library directory.
        install(FILES ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${LIB_FULL_NAME}.lib
                DESTINATION ${INSTALL_LIB})

    endif()

    # Runtime artifacts.
    install(IMPORTED_RUNTIME_ARTIFACTS ${LIB_FULL_NAME}
            RUNTIME_DEPENDENCY_SET ${LIB_DEPS_SET}
            DESTINATION ${INSTALL_BIN})

    # Install the external dependencies.
    install(RUNTIME_DEPENDENCY_SET ${LIB_DEPS_SET}
            PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
            DIRECTORIES ${EXTERNAL_LIBS_PATH} ${INSTALL_LIB}
            DESTINATION ${INSTALL_BIN})

endif()

# **********************************************************************************************************************
