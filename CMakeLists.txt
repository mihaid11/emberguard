cmake_minimum_required(VERSION 3.10)

# Project name and language
project(Emberguard CXX)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define SFML directories for Windows and Unix
if(WIN32)
    if(MSVC)
        set(SFML_DIR "${CMAKE_SOURCE_DIR}/External/Windows/MSVC")
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        set(SFML_DIR "${CMAKE_SOURCE_DIR}/External/Windows/MinGW")
    else()
        message(FATAL_ERROR "Unsupported Windows compiler! Use MSVC or MinGW.")
    endif()
elseif(APPLE)
    set(SFML_DIR "${CMAKE_SOURCE_DIR}/External/macOS")
elseif(UNIX)
    set(SFML_DIR "${CMAKE_SOURCE_DIR}/External/Linux")
endif()

# Define include and library directories for SFML
set(SFML_INCLUDE_DIR "${SFML_DIR}/include")
set(SFML_LIBRARY_DIR "${SFML_DIR}/lib")

include_directories(${SFML_INCLUDE_DIR})

# Define SFML libraries based on the build configuration (Debug or Release)
if(CMAKE_CONFIGURATION_TYPES)  # Multi-config generator (like Visual Studio)
    if(WIN32)
        set(SFML_LIBS 
            optimized ${SFML_LIBRARY_DIR}/sfml-graphics.lib
            optimized ${SFML_LIBRARY_DIR}/sfml-window.lib
            optimized ${SFML_LIBRARY_DIR}/sfml-system.lib
            debug ${SFML_LIBRARY_DIR}/sfml-graphics-d.lib
            debug ${SFML_LIBRARY_DIR}/sfml-window-d.lib
            debug ${SFML_LIBRARY_DIR}/sfml-system-d.lib)
    elseif(APPLE)
        set(SFML_LIBS
            ${SFML_LIBRARY_DIR}/libsfml-graphics.2.6.dylib
            ${SFML_LIBRARY_DIR}/libsfml-window.2.6.dylib
            ${SFML_LIBRARY_DIR}/libsfml-system.2.6.dylib
            ${SFML_LIBRARY_DIR}/libsfml-audio.2.6.dylib
            ${SFML_LIBRARY_DIR}/libsfml-network.2.6.dylib)
    elseif(UNIX)
        set(SFML_LIBS 
            optimized ${SFML_LIBRARY_DIR}/libsfml-graphics.so.2.6.2
            optimized ${SFML_LIBRARY_DIR}/libsfml-window.so.2.6.2
            optimized ${SFML_LIBRARY_DIR}/libsfml-system.so.2.6.2
            debug ${SFML_LIBRARY_DIR}/libsfml-graphics-d.so.2.6.2
            debug ${SFML_LIBRARY_DIR}/libsfml-window-d.so.2.6.2
            debug ${SFML_LIBRARY_DIR}/libsfml-system-d.so.2.6.2)
    endif()
else()  # Single-config generator (Make, Ninja)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        if(WIN32)
            set(SFML_LIBS 
                ${SFML_LIBRARY_DIR}/sfml-graphics-d.lib
                ${SFML_LIBRARY_DIR}/sfml-window-d.lib
                ${SFML_LIBRARY_DIR}/sfml-system-d.lib)
        elseif(APPLE)
            set(SFML_LIBS
                ${SFML_LIBRARY_DIR}/libsfml-graphics.2.6.dylib
                ${SFML_LIBRARY_DIR}/libsfml-window.2.6.dylib
                ${SFML_LIBRARY_DIR}/libsfml-system.2.6.dylib)
        elseif(UNIX)
            set(SFML_LIBS 
                ${SFML_LIBRARY_DIR}/libsfml-graphics-d.so.2.6.2
                ${SFML_LIBRARY_DIR}/libsfml-window-d.so.2.6.2
                ${SFML_LIBRARY_DIR}/libsfml-system-d.so.2.6.2)
        endif()
    else()  # Release build
        if(WIN32)
            set(SFML_LIBS 
                ${SFML_LIBRARY_DIR}/sfml-graphics.lib
                ${SFML_LIBRARY_DIR}/sfml-window.lib
                ${SFML_LIBRARY_DIR}/sfml-system.lib)
        elseif(APPLE)
            set(SFML_LIBS
                ${SFML_LIBRARY_DIR}/libsfml-graphics.2.6.dylib
                ${SFML_LIBRARY_DIR}/libsfml-window.2.6.dylib
                ${SFML_LIBRARY_DIR}/libsfml-system.2.6.dylib)
        elseif(UNIX)
            set(SFML_LIBS 
                ${SFML_LIBRARY_DIR}/libsfml-graphics.so.2.6.2
                ${SFML_LIBRARY_DIR}/libsfml-window.so.2.6.2
                ${SFML_LIBRARY_DIR}/libsfml-system.so.2.6.2)
        endif()
    endif()
endif()

# Glob source files
file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")
add_executable(Emberguard ${SRC_FILES})

# Set working directory for VS Debugger
set_target_properties(Emberguard PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:Emberguard>")

# Link SFML libraries
target_link_libraries(Emberguard ${SFML_LIBS})

# Ensure proper C++17 standard
target_compile_features(Emberguard PRIVATE cxx_std_17)

# Post-build for copying DLLs and assets (for Windows and Unix)
if(WIN32)
    if(CMAKE_CONFIGURATION_TYPES)  # Multi-config (Visual Studio)
        foreach(CONFIG Release Debug)
            file(GLOB SFML_DLLS "${SFML_DIR}/bin/*.dll")
            add_custom_command(TARGET Emberguard POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${SFML_DLLS} $<TARGET_FILE_DIR:Emberguard>
                COMMAND_EXPAND_LISTS
            )
        endforeach()
    else()  # Single-config (Make, Ninja)
        if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "Debug")
            file(GLOB SFML_DLLS "${SFML_DIR}/bin/*.dll")
            add_custom_command(TARGET Emberguard POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${SFML_DLLS} $<TARGET_FILE_DIR:Emberguard>
                COMMAND_EXPAND_LISTS
            )
        endif()
    endif()
elseif(UNIX)
    # Ensure symbolic links for SFML shared libraries
    add_custom_command(TARGET Emberguard POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink 
            ${SFML_LIBRARY_DIR}/libsfml-graphics.so.2.6.2
            ${SFML_LIBRARY_DIR}/libsfml-graphics.so.2.6
        COMMENT "Creating symbolic link: libsfml-graphics.so.2.6 -> libsfml-graphics.so.2.6.2"
        
        COMMAND ${CMAKE_COMMAND} -E create_symlink 
            ${SFML_LIBRARY_DIR}/libsfml-window.so.2.6.2
            ${SFML_LIBRARY_DIR}/libsfml-window.so.2.6
        COMMENT "Creating symbolic link: libsfml-window.so.2.6 -> libsfml-window.so.2.6.2"
        
        COMMAND ${CMAKE_COMMAND} -E create_symlink 
            ${SFML_LIBRARY_DIR}/libsfml-system.so.2.6.2
            ${SFML_LIBRARY_DIR}/libsfml-system.so.2.6
        COMMENT "Creating symbolic link: libsfml-system.so.2.6 -> libsfml-system.so.2.6.2"
    )
endif()

# Post-build step for copying assets directory
add_custom_command(TARGET Emberguard POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:Emberguard>/assets
)

