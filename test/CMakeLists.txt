cmake_minimum_required(VERSION 3.21)

#-------------------------------------------------------------------------------
# Add test data

include(FetchContent)

# # Fetch Silesia corpus as test data
# FetchContent_Declare(
#   silesia_corpus
#   URL https://sun.aei.polsl.pl/~sdeor/corpus/silesia.zip
#   URL_HASH MD5=c240c17d6805fb8a0bde763f1b94cd99
#   SOURCE_DIR data/silesia
#   DOWNLOAD_EXTRACT_TIMESTAMP FALSE
# )
# # Fetch Canterbury corpus as test data
# FetchContent_Declare(
#   canterbury_corpus
#   URL https://corpus.canterbury.ac.nz/resources/cantrbry.zip
#   URL_HASH MD5=0308697233961e9217fd52be64ca5e85
#   SOURCE_DIR data/canterbury
#   DOWNLOAD_EXTRACT_TIMESTAMP FALSE
# )
# FetchContent_MakeAvailable(silesia_corpus canterbury_corpus)

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
# Create a library target for the Catch2 test framework

set(CATCH_CONFIG_PREFIX_ALL ON CACHE BOOL "Prefix all Catch2 macros with CATCH_")
add_subdirectory(Catch2)

#-------------------------------------------------------------------------------
# Define targets

set(TARGET_SRC
    ${PROJECT_SOURCE_DIR}/src/compression/lz4.cpp
    ${PROJECT_SOURCE_DIR}/src/compression/lzss.cpp
)

set(TARGET_INCLUDES
    ${PROJECT_SOURCE_DIR}/src
)

file(GLOB TESTS_SRC CONFIGURE_DEPENDS "*.cpp")

set(TARGET_NAME unit_tests)
add_executable(${TARGET_NAME} ${TARGET_SRC} ${TESTS_SRC})
target_link_libraries(${TARGET_NAME} PRIVATE OpenMP::OpenMP_CXX Catch2::Catch2WithMain pthread)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${TARGET_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/Catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(${TARGET_NAME})
