####  FindCET.cmake:
####     Load the CET interface to the Fermilab UPS system.
####      (Several of our optional packages have a number of dependencies which are best resolved via UPS.)
####     This requires a lot of shenanigans because the CMake UPS interface assumes
####     you want to build your package using CET as well, and we don't.
####
####   Original author:  J. Wolcott <jwolcott@fnal.gov>
####   Date:             Nov. 1, 2018

# this file will be included by several others
include_guard(GLOBAL)

# cetbuildtools contains our cmake modules
set(CETBUILDTOOLS_VERSION $ENV{CETBUILDTOOLS_VERSION})
if (NOT CETBUILDTOOLS_VERSION)
	MESSAGE (FATAL_ERROR "ERROR: setup cetbuildtools to get the cmake modules")
endif()

set(CMAKE_MODULE_PATH $ENV{CANVAS_DIR}/Modules
    $ENV{CETBUILDTOOLS_DIR}/Modules
    ${CMAKE_MODULE_PATH})
include(CetCMakeEnv)
#message(STATUS "$CMAKE_PROJECT_NAME = '${CMAKE_PROJECT_NAME}'")
#message(STATUS "$product = '${product}'")
#message(STATUS "$PROJECT_BINARY_DIR = '${PROJECT_BINARY_DIR}'")

# CET seems to require a fortran compiler to be set <eyeroll>
enable_language(Fortran)

# to make CET happy (it can't handle the path component)
get_filename_component(cc ${CMAKE_C_COMPILER} NAME)
get_filename_component(cxx ${CMAKE_CXX_COMPILER} NAME)
get_filename_component(fc  ${CMAKE_Fortran_COMPILER} NAME)

# if we were using the CET build tools to build, this file would be created for us.
# we just want to use CET to find all the dependencies, so...
file (WRITE ${PROJECT_BINARY_DIR}/cetpkg_variable_report "
CETPKG_NAME             ${CMAKE_PROJECT_NAME}
CETPKG_VERSION          ${VERSION}
CETPKG_DEFAULT_VERSION  none
CETPKG_FLAVOR           ${NOVARWGT_UPS_FLAVOR}
CETPKG_QUAL             ${NOVARWGT_UPS_COMPILER}:${NOVARWGT_BUILD_TYPE}
CETPKG_SOURCE           ${PROJECT_SOURCE_DIR}
CETPKG_BUILD            ${PROJECT_BINARY_DIR}
CETPKG_CC               ${cc}
CETPKG_CXX              ${cxx}
CETPKG_FC               ${fc}
CETPKG_ONLY_FOR_BUILD   cetbuildtools;
")

# forge the architecture so we find the package in UPS...
#cet_cmake_env(Linux64bit+2.6-2.12)
cet_cmake_env(noarch)