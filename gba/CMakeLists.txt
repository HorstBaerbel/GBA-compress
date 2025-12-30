cmake_minimum_required(VERSION 3.21)

# Use GBA toolchain
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/GBA-demo-framework/3ds-cmake/DevkitArmGBA.cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/GBA-demo-framework/3ds-cmake/cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/GBA-demo-framework/cmake)

project(gba_examples)

# ASM must be enabled to support .S files
enable_language(ASM)
# Include all the macros and tools needed for GBA development
include(ToolsGBA)
# Using some special compiler flags for GBA
include(CompilerFlagsGBA)

set(GBACOMP_PATH "${CMAKE_CURRENT_LIST_DIR}/../build/src/gbacomp")

macro(target_gbacomp_file _listName _outname _infiles _options)
	# split options and arguments up
	separate_arguments(_option_list NATIVE_COMMAND ${_options})
	separate_arguments(_infile_list NATIVE_COMMAND ${_infiles})
	# check if the input file is a wildcard
	file(GLOB _globbed_infiles CONFIGURE_DEPENDS ${CMAKE_CURRENT_LIST_DIR}/${_infile_list})
	add_custom_command(
		OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/data/${_outname}"
		COMMAND "${GBACOMP_PATH}" ${_option_list} ${_infile_list} data/${_outname}
		DEPENDS ${_globbed_infiles}
		WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
		COMMAND_EXPAND_LISTS
	)
	LIST(APPEND ${_listName} "${CMAKE_CURRENT_SOURCE_DIR}/data/${_outname}")
endmacro()

# Video example sources --------------------------------------------
LIST(APPEND TARGET_SOURCE_FILES
	GBA-demo-framework/src/tui.cpp
	GBA-demo-framework/src/memory/memcpy.s
	GBA-demo-framework/src/memory/memset.s
	GBA-demo-framework/src/print/itoa.cpp
	GBA-demo-framework/src/print/output.cpp
	GBA-demo-framework/src/sys/input.cpp
	GBA-demo-framework/src/sys/intdispatcher.s
	GBA-demo-framework/src/sys/interrupts.cpp
	GBA-demo-framework/src/compression/bios.cpp
	GBA-demo-framework/src/compression/lz4_asm.s
	GBA-demo-framework/src/compression/lz77_asm.s
	data/font_8x8.c
	data/images.s
	gbacomp.cpp
)
set_source_files_properties(data/images.s OBJECT_DEPENDS "${CMAKE_SOURCE_DIR}/data/image_lz4_0;${CMAKE_SOURCE_DIR}/data/image_lz77_0")
set_source_files_properties(data/images.s OBJECT_DEPENDS "${CMAKE_SOURCE_DIR}/data/image_lz4_1;${CMAKE_SOURCE_DIR}/data/image_lz77_1")
set_source_files_properties(data/images.s OBJECT_DEPENDS "${CMAKE_SOURCE_DIR}/data/image_lz4_2;${CMAKE_SOURCE_DIR}/data/image_lz77_2")
set_source_files_properties(data/images.s OBJECT_DEPENDS "${CMAKE_SOURCE_DIR}/data/image_lz4_3;${CMAKE_SOURCE_DIR}/data/image_lz77_3")
set_source_files_properties(data/images.s OBJECT_DEPENDS "${CMAKE_SOURCE_DIR}/data/image_lz4_4;${CMAKE_SOURCE_DIR}/data/image_lz77_4")
set_source_files_properties(data/images.s OBJECT_DEPENDS "${CMAKE_SOURCE_DIR}/data/image_lz4_5;${CMAKE_SOURCE_DIR}/data/image_lz77_5")
set_source_files_properties(data/images.s OBJECT_DEPENDS "${CMAKE_SOURCE_DIR}/data/image_lz4_6;${CMAKE_SOURCE_DIR}/data/image_lz77_6")
set_source_files_properties(data/images.s OBJECT_DEPENDS "${CMAKE_SOURCE_DIR}/data/image_lz4_7;${CMAKE_SOURCE_DIR}/data/image_lz77_7")

# Add image source files to be converted with gbacomp here. Paths with spaces need escaping (\"\")
# These files will be added to TARGET_DATA_FILES
target_gbacomp_file(TARGET_DATA_FILES "image_lz4_0" "\"../data/flower_foveon_240x160_bgr555.raw\"" "--lz4 --force")
target_gbacomp_file(TARGET_DATA_FILES "image_lz4_1" "\"../data/artificial_240x160_bgr555.raw\"" "--lz4 --force")
target_gbacomp_file(TARGET_DATA_FILES "image_lz4_2" "\"../data/BigBuckBunny_40_240x160_bgr555.raw\"" "--lz4 --force")
target_gbacomp_file(TARGET_DATA_FILES "image_lz4_3" "\"../data/BigBuckBunny_361_240x160_bgr555.raw\"" "--lz4 --force")
target_gbacomp_file(TARGET_DATA_FILES "image_lz4_4" "\"../data/gradient_240x160_bgr555.raw\"" "--lz4 --force")
target_gbacomp_file(TARGET_DATA_FILES "image_lz4_5" "\"../data/nightshot_iso_100_240x160_bgr555.raw\"" "--lz4 --force")
target_gbacomp_file(TARGET_DATA_FILES "image_lz4_6" "\"../data/squish_240x160_bgr555.raw\"" "--lz4 --force")
target_gbacomp_file(TARGET_DATA_FILES "image_lz4_7" "\"../data/TearsOfSteel_1200_240x160_bgr555.raw\"" "--lz4 --force")
target_gbacomp_file(TARGET_DATA_FILES "image_lz77_0" "\"../data/flower_foveon_240x160_bgr555.raw\"" "--lz10 --force")
target_gbacomp_file(TARGET_DATA_FILES "image_lz77_1" "\"../data/artificial_240x160_bgr555.raw\"" "--lz10 --force")
target_gbacomp_file(TARGET_DATA_FILES "image_lz77_2" "\"../data/BigBuckBunny_40_240x160_bgr555.raw\"" "--lz10 --force")
target_gbacomp_file(TARGET_DATA_FILES "image_lz77_3" "\"../data/BigBuckBunny_361_240x160_bgr555.raw\"" "--lz10 --force")
target_gbacomp_file(TARGET_DATA_FILES "image_lz77_4" "\"../data/gradient_240x160_bgr555.raw\"" "--lz10 --force")
target_gbacomp_file(TARGET_DATA_FILES "image_lz77_5" "\"../data/nightshot_iso_100_240x160_bgr555.raw\"" "--lz10 --force")
target_gbacomp_file(TARGET_DATA_FILES "image_lz77_6" "\"../data/squish_240x160_bgr555.raw\"" "--lz10 --force")
target_gbacomp_file(TARGET_DATA_FILES "image_lz77_7" "\"../data/TearsOfSteel_1200_240x160_bgr555.raw\"" "--lz10 --force")

LIST(APPEND TARGET_INCLUDE_DIRS
	${DEVKITARM}/arm-none-eabi/include
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/data
	${CMAKE_CURRENT_SOURCE_DIR}/GBA-demo-framework/src
)

# gbacomp example
add_executable(gbacomp ${TARGET_SOURCE_FILES} ${TARGET_DATA_FILES}) # Create the elf file
add_gba_executables(gbacomp "GBACOMP" "GBAC" "HB" 1) # Generate the .gba from the elf
add_map_file(gbacomp)
add_sections_file(gbacomp)
target_include_directories(gbacomp PRIVATE ${TARGET_INCLUDE_DIRS})
#target_link_directories(gbacomp PRIVATE  ${LIB_DIRS})
#target_link_libraries(gbacomp ${LIBS}) # Link the application and the demo framework
