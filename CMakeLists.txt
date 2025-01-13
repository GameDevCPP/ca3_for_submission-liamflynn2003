cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Space-Hunt)

#### Setup Directories ####
set(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

add_custom_target(copy_resources ALL COMMAND ${CMAKE_COMMAND}
        -E copy_directory
        "${PROJECT_SOURCE_DIR}/res"
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/res
)

#### Add External Dependencies ####
add_subdirectory("lib/sfml" EXCLUDE_FROM_ALL)
set(SFML_INCS "lib/sfml/include")

add_subdirectory("lib/b2d/Box2D")
set(B2D_INCS "lib/b2d/Box2D/")

#### Add Engine and Library Projects ####
file(GLOB_RECURSE RESOURCE_FILES "res/*.*")

# Engine
file(GLOB_RECURSE SOURCE_FILES engine/*.cpp engine/*.h)
add_library(lib_engine STATIC ${SOURCE_FILES} ${RESOURCE_FILES})
target_include_directories(lib_engine INTERFACE "${CMAKE_SOURCE_DIR}/engine/" PUBLIC SYSTEM ${B2D_INCS})
target_link_libraries(lib_engine lib_tile_level_loader lib_maths lib_ecm lib_sound Box2D sfml-graphics sfml-audio)

# Tile Loader
file(GLOB_RECURSE SOURCE_FILES lib_tile_level_loader/*.cpp lib_tile_level_loader/*.h)
add_library(lib_tile_level_loader STATIC ${SOURCE_FILES})
target_include_directories(lib_tile_level_loader INTERFACE "${CMAKE_SOURCE_DIR}/lib_tile_level_loader/")
target_link_libraries(lib_tile_level_loader lib_maths sfml-graphics)

# Maths
add_library(lib_maths INTERFACE)
target_sources(lib_maths INTERFACE "${CMAKE_SOURCE_DIR}/lib_maths/maths.h")
target_include_directories(lib_maths INTERFACE "${CMAKE_SOURCE_DIR}/lib_maths" SYSTEM INTERFACE ${SFML_INCS})

# ECM
file(GLOB_RECURSE SOURCE_FILES lib_ecm/*.cpp lib_ecm/*.h)
add_library(lib_ecm STATIC ${SOURCE_FILES}
        pathfinding/AStar.cpp)
target_include_directories(lib_ecm INTERFACE "${CMAKE_SOURCE_DIR}/lib_ecm/")
target_link_libraries(lib_ecm PRIVATE lib_maths)

# Sound
file(GLOB_RECURSE SOUND_FILES lib_sound/*.cpp lib_sound/*.h)
add_library(lib_sound STATIC ${SOUND_FILES})
target_include_directories(lib_sound INTERFACE "${CMAKE_SOURCE_DIR}/lib_sound/")
target_link_libraries(lib_sound sfml-audio)


#### Space Hunt ####
file(GLOB_RECURSE SOURCES Space-Hunt/*.cpp Space-Hunt/*.h)
file(GLOB_RECURSE CMPNTS Space-Hunt/components/*.cpp Space-Hunt/components/*.h)
file(GLOB_RECURSE SCENES Space-Hunt/scenes/*.cpp Space-Hunt/scenes/*.h)
add_executable(Space_Hunt ${SOURCES} ${RESOURCE_FILES})
source_group("components" FILES ${CMPNTS})
source_group("resources" FILES ${RESOURCE_FILES})
target_link_libraries(Space_Hunt lib_engine)

# Resource copying
add_dependencies(Space_Hunt copy_resources)

set_target_properties(Space_Hunt
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$(Configuration)
)
