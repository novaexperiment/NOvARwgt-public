cmake_minimum_required(VERSION 3.10)

############   top level config

project(NOvARwgt)
set(VERSION "v1.0.1")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_SKIP_RPATH false)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/Modules")

# the BUILD_TYPE and UPS_... vars are only used when working with UPS,
# but that means they're needed in the FindCET.cmake called by some of the external dependencies
if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
	set(NOVARWGT_BUILD_TYPE "debug")
else ()
	set(NOVARWGT_BUILD_TYPE "prof")
endif ()

# There are a couple of linker flags that are different between Clang with LLVM and GCC
# so we account for those here.
if( ${CMAKE_C_COMPILER_ID} MATCHES "Clang")
  # OS X doesn't use the --no-undefined flag, but uses -undefined,error instead
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-undefined,error")
else()  
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined")
  # by default the linker will drop references to shared libraries that it finds it doesn't actually need
  # to resolve the dependencies in whatever library it's building. however, this is a problem for us,
  # because GENIE doesn't correctly build its libraries' interdependencies into its own libraries, which means
  # that we are forced to link all its libraries into everything we make or the GENIE libraries will have symbols
  # that can't be resolved because they're in other GENIE libraries.  if the linker is allowed to drop dependencies that
  # it doesn't find are necessary to build our library, then that problem will crop up anytime we link one GENIE library
  # that interally relies on another.  so, we disable the default linker behavior.
  set( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-as-needed" )
endif()


# note that these can be overridden from the cmake command line with -DNOVARWGT_UPS_FLAVOR="blah" etc.
set(NOVARWGT_UPS_FLAVOR "Linux64bit+2.6-2.12" CACHE STRING "UPS flavor to build NOvARwgt with (corresponds to OS).  Popular options: Linux64bit+2.6-2.12, Darwin64Bit+17")
set(NOVARWGT_UPS_OS "slf6.x86_64" CACHE STRING "OS where NOvARwgt is being built.  Popular options: slf6.x86_64, d17.x86_64")
set(NOVARWGT_UPS_COMPILER "e15" CACHE STRING "UPS qualifier for compiler used to build.  See https://cdcvs.fnal.gov/redmine/projects/cet-is-public/wiki/AboutQualifiers")

###########   now load the externals we need
###########     (custom cmake files for finding them in cmake/Modules)

# look for ROOT first.
find_package(ROOT REQUIRED COMPONENTS EG EGPythia6)

# ROOT does things differently depending on whether it's built for C++17 or not :-\
#execute_process(COMMAND "root-config --cflags | sed -n 's/.*c++\([0-9][0-9]\).*/\\1/pg'" OUTPUT_VARIABLE ROOT_CXX_STANDARD)
string(REGEX REPLACE ".*c\\+\\+([0-9][0-9]).*" "\\1" ROOT_CXX_STANDARD ${ROOT_CXX_FLAGS})
if(ROOT_CXX_STANDARD LESS_EQUAL 14)
	message(WARNING "Overriding default C++ standard version (${CMAKE_CXX_STANDARD}) 14 (lowest supported by NOvARwgt)") #with version deduced from ROOT: ${ROOT_CXX_STANDARD}")
	set(CMAKE_CXX_STANDARD 14)
endif()


# Fermi products which are often useful
option(NOVARWGT_USE_CETLIB "Use FNAL CETLib?" Off)
if(NOVARWGT_USE_CETLIB)
	find_package(CETLib)
endif()

# GENIE
option(NOVARWGT_USE_GENIE "Build support for GENIE?" On)
if(NOVARWGT_USE_GENIE)
	find_package(GENIE)
else()
	set(NOVARWGT_GENIE_QUAL "nogenie")
endif()

# FNAL NuSimData
option(NOVARWGT_USE_NUSIMDATA "Build support for FNAL NuSimData?" On)
if(NOVARWGT_USE_NUSIMDATA)
	find_package(NuSimData)
endif()

###########   source can be installed with build if user desires
option(NOVARWGT_INSTALL_SOURCE "Install source in target directory?" On)

###########   build structure can be altered to better suit becoming a UPS product

option(NOVARWGT_INSTALL_UPS "Include UPS product files in installation?" false)

if(NOVARWGT_INSTALL_UPS)
	set(UPS_PROD_NAME_UC "NOVARWGT")

	set(qualifiers ${NOVARWGT_UPS_COMPILER} ${NOVARWGT_BUILD_TYPE} ${NOVARWGT_GENIE_QUAL})
	list(SORT qualifiers)
	string(REPLACE ";" "." qualifiers_sorted "${qualifiers}")  # list(JOIN ...) doesn't exist until CMake 3.12 :(
	set(NOVARWGT_UPS_SUBDIR "${NOVARWGT_UPS_OS}.${qualifiers_sorted}")
	set(UPS_PROD_QUALIFIERS "${NOVARWGT_UPS_COMPILER}:${NOVARWGT_GENIE_QUAL}:${NOVARWGT_BUILD_TYPE}")
	message(STATUS "Building with qualifiers = ${UPS_PROD_QUALIFIERS}")

endif()


if (NOVARWGT_UPS_SUBDIR)
	set(TARGET_LIBDIR ${NOVARWGT_UPS_SUBDIR}/lib)
	set(TARGET_BINDIR ${NOVARWGT_UPS_SUBDIR}/bin)
else()
	set(TARGET_LIBDIR lib)
	set(TARGET_BINDIR bin)
endif()

###########   directory configuration

include_directories(
		${ROOT_INCLUDE_DIRS}
		inc
)

###########   subdirs

add_subdirectory(data)
add_subdirectory(inc)
add_subdirectory(scripts)
add_subdirectory(src)
add_subdirectory(src/test)
add_subdirectory(ups)

###########   install the README in the top dir

install(FILES README.md INSTALL.md HACKING.md CHANGELOG.md DESTINATION .)
