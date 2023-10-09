

# **********************************************************************************************************************
# Updated 03/10/2023
# **********************************************************************************************************************

# **********************************************************************************************************************

# Add the tests in the subdirectories.
macro_add_subdirs(${CMAKE_CURRENT_SOURCE_DIR})

# **********************************************************************************************************************
# BASIC EXAMPLES

# ----------------------------------------------------------------------------------------------------------------------
# EXAMPLE AMELAS CONTROLLER CLIENT

# App config.
set(APP_CLIENT_EXAMPLE "ExampleAmelasControllerClient")
set(APP_BUILD_FOLDER ${CMAKE_BINARY_DIR}/bin/Examples)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${APP_BUILD_FOLDER})

# Get the source files for server.
file(GLOB_RECURSE SOURCES ExampleAmelasControllerClient.cpp)

# Clean alias files and templates.
set(ALIAS "")
set(TEMPLTS "")

# Set libraries. For win32 only.
if (WIN32)
    set(LIBRARIES ${LIB_FULL_NAME})
endif()

# Setup the client launcher.
macro_setup_deploy_launcher("${APP_CLIENT_EXAMPLE}" "${INSTALL_BIN}" "${LIB_DEPS_SET}")

# Include the server common dirs.
target_include_directories(${APP_CLIENT_EXAMPLE} PRIVATE
                           ${CMAKE_SOURCE_DIR}/includes)

# In mingw better do static linking of the libgcc, libwinpthread and libstd.
if (MINGW)
   target_link_libraries(${APP_CLIENT_EXAMPLE} PRIVATE -static-libgcc -static-libstdc++ -static -lpthread)
endif()

# ----------------------------------------------------------------------------------------------------------------------
# EXAMPLE AMELAS CONTROLLER SERVER

# App config.
set(APP_SERVER_EXAMPLE "ExampleAmelasControllerServer")
set(APP_BUILD_FOLDER ${CMAKE_BINARY_DIR}/bin/Examples)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${APP_BUILD_FOLDER})

# Get the source files for server.
file(GLOB_RECURSE SOURCES ExampleAmelasControllerServer.cpp)

# Clean alias files and templates.
set(ALIAS "")
set(TEMPLTS "")

# Set libraries. For win32 only.
if (WIN32)
    set(LIBRARIES ${LIB_FULL_NAME})
endif()

# Setup the client launcher.
macro_setup_deploy_launcher("${APP_SERVER_EXAMPLE}" "${INSTALL_BIN}" "${LIB_DEPS_SET}")

# Include the server common dirs.
target_include_directories(${APP_SERVER_EXAMPLE} PRIVATE
                           ${CMAKE_SOURCE_DIR}/includes)

# In mingw better do static linking of the libgcc, libwinpthread and libstd.
if (MINGW)
  target_link_libraries(${APP_SERVER_EXAMPLE} PRIVATE -static-libgcc -static-libstdc++ -static -lpthread)
endif()

# **********************************************************************************************************************
