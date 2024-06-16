cmake_minimum_required(VERSION 3.20)
# Set the Project Name.
set(PROJECT_NAME                Dumper-7)
# Set the name of the target operating system.
set(CMAKE_SYSTEM_NAME           Windows)

if (UNIX) # If compiling on Linux, manually configure MingW-64.
    set(CMAKE_C_COMPILER        x86_64-w64-mingw32-gcc)
    set(CMAKE_CXX_COMPILER      x86_64-w64-mingw32-g++)
    # Set the Toolchain to MinGW on Linux.
    set(CMAKE_FIND_ROOT_PATH    /usr/x86_64-w64-mingw32)
    link_directories(${CMAKE_FIND_ROOT_PATH}/sys-root/mingw/lib) # TODO: Fix to also work with MinGW on Windows.
    # adjust the default behavior of the FIND_XXX() commands:
    # Never search for programs in the host environment
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    # Search for headers and libraries only in the target environment
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
endif (UNIX)

# Set our output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Binaries/${CMAKE_SYSTEM_NAME})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Binaries/${CMAKE_SYSTEM_NAME})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Binaries/${CMAKE_SYSTEM_NAME})

# Set our intermediate directories.
set(CMAKE_BINARY_DIR               ${CMAKE_CURRENT_SOURCE_DIR}/Intermediate/${CMAKE_BUILD_TYPE})
set(CMAKE_BUILD_FILES_DIRECTORY    ${CMAKE_CURRENT_SOURCE_DIR}/Intermediate/${CMAKE_BUILD_TYPE})

# Set our compile method to Ninja.
set(CMAKE_GENERATOR Ninja)
set(CMAKE_CXX_STANDARD 23)

# Set a definition for a debug build through CMake.
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")


## ZSTD
set(ZSTD_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/Dumper/ExternalDependencies/Compression")
add_library(zstd STATIC
        ${ZSTD_SOURCE_DIR}/zstd.c
        ${ZSTD_SOURCE_DIR}/zstd.h
)

## DSGen
set(DSGEN_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/Dumper/ExternalDependencies/Dumpspace")
add_library(dsgen STATIC
        ${DSGEN_SOURCE_DIR}/DSGen.cpp
        ${DSGEN_SOURCE_DIR}/DSGen.h
)

## JSon
set(JSON_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/Dumper/ExternalDependencies/Json")
add_library(json STATIC
        ${JSON_SOURCE_DIR}/json.hpp
)
set_target_properties(json PROPERTIES LINKER_LANGUAGE CXX)

# Now finally export our library file.
## Setup the project and set a few variables
project(Dumper-7)
set(PROJECT_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/Dumper")
add_library(Dumper-7 SHARED
        ${PROJECT_SOURCE_DIR}/CollisionManager.cpp
        ${PROJECT_SOURCE_DIR}/CollisionManager.h
        ${PROJECT_SOURCE_DIR}/CollisionManagerTest.h
        ${PROJECT_SOURCE_DIR}/CppGenerator.cpp
        ${PROJECT_SOURCE_DIR}/CppGenerator.h
        ${PROJECT_SOURCE_DIR}/CppGeneratorTest.h
        ${PROJECT_SOURCE_DIR}/DependencyManager.cpp
        ${PROJECT_SOURCE_DIR}/DependencyManager.h
        ${PROJECT_SOURCE_DIR}/DumpspaceGenerator.cpp
        ${PROJECT_SOURCE_DIR}/DumpspaceGenerator.h
        ${PROJECT_SOURCE_DIR}/EnumManager.cpp
        ${PROJECT_SOURCE_DIR}/EnumManager.h
        ${PROJECT_SOURCE_DIR}/EnumManagerTest.h
        ${PROJECT_SOURCE_DIR}/Enums.h
        ${PROJECT_SOURCE_DIR}/EnumWrapper.cpp
        ${PROJECT_SOURCE_DIR}/EnumWrapper.h
        ${PROJECT_SOURCE_DIR}/Generator.cpp
        ${PROJECT_SOURCE_DIR}/Generator.h
        ${PROJECT_SOURCE_DIR}/GeneratorTest.h
        ${PROJECT_SOURCE_DIR}/HashStringTable.cpp
        ${PROJECT_SOURCE_DIR}/HashStringTable.h
        ${PROJECT_SOURCE_DIR}/HashStringTableTest.h
        ${PROJECT_SOURCE_DIR}/IDAMappingGenerator.cpp
        ${PROJECT_SOURCE_DIR}/IDAMappingGenerator.h
        ${PROJECT_SOURCE_DIR}/main.cpp
        ${PROJECT_SOURCE_DIR}/MappingGenerator.cpp
        ${PROJECT_SOURCE_DIR}/MappingGenerator.h
        ${PROJECT_SOURCE_DIR}/MemberManager.cpp
        ${PROJECT_SOURCE_DIR}/MemberManager.h
        ${PROJECT_SOURCE_DIR}/MemberManagerTest.h
        ${PROJECT_SOURCE_DIR}/MemberWrappers.cpp
        ${PROJECT_SOURCE_DIR}/MemberWrappers.h
        ${PROJECT_SOURCE_DIR}/NameArray.cpp
        ${PROJECT_SOURCE_DIR}/NameArray.h
        ${PROJECT_SOURCE_DIR}/ObjectArray.cpp
        ${PROJECT_SOURCE_DIR}/ObjectArray.h
        ${PROJECT_SOURCE_DIR}/OffsetFinder.h
        ${PROJECT_SOURCE_DIR}/Offsets.cpp
        ${PROJECT_SOURCE_DIR}/Offsets.h
        ${PROJECT_SOURCE_DIR}/PackageManager.cpp
        ${PROJECT_SOURCE_DIR}/PackageManager.h
        ${PROJECT_SOURCE_DIR}/PackageManagerTest.h
        ${PROJECT_SOURCE_DIR}/PredefinedMembers.h
        ${PROJECT_SOURCE_DIR}/Settings.h
        ${PROJECT_SOURCE_DIR}/StructManager.cpp
        ${PROJECT_SOURCE_DIR}/StructManager.h
        ${PROJECT_SOURCE_DIR}/StructManagerTest.h
        ${PROJECT_SOURCE_DIR}/StructWrapper.cpp
        ${PROJECT_SOURCE_DIR}/StructWrapper.h
        ${PROJECT_SOURCE_DIR}/TestBase.h
        ${PROJECT_SOURCE_DIR}/UnrealObjects.cpp
        ${PROJECT_SOURCE_DIR}/UnrealObjects.h
        ${PROJECT_SOURCE_DIR}/UnrealTypes.cpp
        ${PROJECT_SOURCE_DIR}/UnrealTypes.h
        ${PROJECT_SOURCE_DIR}/Utils.h
)
## Add the dependencies we set up previously.
# NOTE: Add "" to the "add_dependencies" section when you finally get that shit working.
add_dependencies(Dumper-7 zstd dsgen json)
target_link_libraries(Dumper-7 zstd dsgen json)
## Set up our linker flags for compiling.
set(CMAKE_SHARED_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static -municode")
set_target_properties(Dumper-7 PROPERTIES PREFIX "" OUTPUT_NAME Dumper-7 SUFFIX ".asi")
## Now we delete any miscellaneous .a static library files for release builds, as we don't need them.
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E remove $<TARGET_FILE_DIR:${PROJECT_NAME}>/*.a)
endif()