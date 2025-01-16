cmake_minimum_required(VERSION 3.21)

#-------------------------------------------------------------------------------
# Add required libraries

find_package(OpenMP REQUIRED)

#-------------------------------------------------------------------------------
# Set up compiler flags

if(MSVC)
    set(CMAKE_DEBUG_POSTFIX "d")
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP") #multi-processor compilation
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP") #multi-processor compilation
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

#-------------------------------------------------------------------------------
# Define sources

file(GLOB_RECURSE HELPERS_SRC CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*/*.cpp ${PROJECT_SOURCE_DIR}/src/*/*.h)

list(APPEND GBACOMP_SRC
    gbacomp.cpp
    options.cpp
    ${HELPERS_SRC}
)

#-------------------------------------------------------------------------------
# Define targets

include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/cxxopts ${PROJECT_SOURCE_DIR}/glob ${PROJECT_SOURCE_DIR}/src)
add_executable(gbacomp ${GBACOMP_SRC})
target_link_libraries(gbacomp OpenMP::OpenMP_CXX stdc++fs pthread)

#-------------------------------------------------------------------------------
# Define install files for CPack

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/gbacomp"
    "${PROJECT_SOURCE_DIR}/README.md"
    "${PROJECT_SOURCE_DIR}/LICENSE"
    DESTINATION .
)

# Tell CPack to create a zip file with our project name
set(CPACK_GENERATOR "ZIP")
# Tell CPack not to put everything inside an enclosing directory.
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
# Apparently this should be always on but isn't for backwards compatibility.
set(CPACK_VERBATIM_VARIABLES YES)
include(CPack)
