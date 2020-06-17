####  FindCETLib.cmake:
####     Locate the CET library tools.
####
####   Original author:  J. Wolcott <jwolcott@fnal.gov>
####   Date:             Nov. 1, 2018


find_package(CET)

# if CETLIB is available, it can be used to help locate files.
# it depends on CETLIB_EXCEPT...
find_ups_product(cetlib)

set(CETLIB_INC "$ENV{CETLIB_INC}")
if(CETLIB_INC)
	set(CETLIB_LIBDIR "$ENV{CETLIB_LIB}")
	set(CETLIB_EXCEPT_LIBDIR "$ENV{CETLIB_EXCEPT_LIB}")
endif()

if(CETLIB_EXCEPT_LIBDIR)
	message(STATUS "yep, we have CETLIB_EXCEPT")
else()
	message(WARNING "no CETLIB_EXCEPT")
endif()

if (CETLIB_EXCEPT_LIBDIR AND CETLIB_LIBDIR)
	find_package(Boost REQUIRED)
	message(STATUS "Loading CETLib")
	add_definitions(-DUSE_CETLIB)
	# needed because while find_ups_product() sets the env vars, it doesn't change CMake's search path,
	# and thus the library's internal dependency is never resolved
	link_directories(${CETLIB_EXCEPT_LIBDIR})
	find_library(cetlib_loc "cetlib" PATHS ENV CETLIB_LIBDIR)
	set(CETLIB_LIBRARIES ${cetlib_loc})
endif()

message(STATUS "$CETLIB_LIBRARIES = ${CETLIB_LIBRARIES}")
