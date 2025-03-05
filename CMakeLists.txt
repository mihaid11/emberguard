    cmake_minimum_required(VERSION 3.10)

# Project name and language
project(Emberguard CXX)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    if(MSVC)
        set(SFML_DIR "${CMAKE_SOURCE_DIR}/External/Windows/MSVC")
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        set(SFML_DIR "${CMAKE_SOURCE_DIR}/External/Windows/MinGW")
    else()
        message(FATAL_ERROR "Unsupported Windows compiler! Use MSVC or MinGW.")
    endif()
elseif(UNIX)
    set(SFML_DIR "${CMAKE_SOURCE_DIR}/External/Linux")
endif()

set(SFML_INCLUDE_DIR "${SFML_DIR}/include")
set(SFML_LIBRARY_DIR "${SFML_DIR}/lib")

include_directories(${SFML_INCLUDE_DIR})

if(CMAKE_CONFIGURATION_TYPES)  # Multi-config generator (like Visual Studio)
    set(SFML_LIBS 
        optimized ${SFML_LIBRARY_DIR}/sfml-graphics.lib
        optimized ${SFML_LIBRARY_DIR}/sfml-window.lib
        optimized ${SFML_LIBRARY_DIR}/sfml-system.lib
        debug ${SFML_LIBRARY_DIR}/sfml-graphics-d.lib
        debug ${SFML_LIBRARY_DIR}/sfml-window-d.lib
        debug ${SFML_LIBRARY_DIR}/sfml-system-d.lib)
else()  # Single-config generator (Make, Ninja)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(SFML_LIBS 
            ${SFML_LIBRARY_DIR}/sfml-graphics-d.lib
            ${SFML_LIBRARY_DIR}/sfml-window-d.lib
            ${SFML_LIBRARY_DIR}/sfml-system-d.lib)
    else()
        set(SFML_LIBS 
            ${SFML_LIBRARY_DIR}/sfml-graphics.lib
            ${SFML_LIBRARY_DIR}/sfml-window.lib
            ${SFML_LIBRARY_DIR}/sfml-system.lib)
    endif()
endif()

file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")
add_executable(Emberguard ${SRC_FILES})

set_target_properties(Emberguard PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:Emberguard>")

target_link_libraries(Emberguard 
    ${SFML_LIBS}
)
target_compile_features(Emberguard PRIVATE cxx_std_17)

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
endif()


add_custom_command(TARGET Emberguard POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:Emberguard>/assets
)

