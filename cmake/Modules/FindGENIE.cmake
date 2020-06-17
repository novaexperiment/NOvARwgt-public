####  FindGENIE.cmake:
####     Locate the GENIE includes and libraries.
####
####   Original author:  J. Wolcott <jwolcott@fnal.gov>
####   Date:             Nov. 1, 2018

set(GENIE "$ENV{GENIE}")
set(PYTHIA6_LIBDIR "$ENV{PYTHIA6_LIBRARY}")
if(PYTHIA6_LIBDIR)
else()
	message(WARNING "$GENIE defined but $PYTHIA6_LIBRARY unset; GENIE support disabled." )
	unset(GENIE)
endif()
if(GENIE)
	set(USE_GENIE TRUE)
	file(READ "$ENV{GENIE}/VERSION" GENIE_VERSION)
	# GENIE's reweight machinery is a separate product from v3
	string(COMPARE GREATER "${GENIE_VERSION}" "3.0.0" genie_v3)
	if(genie_v3)
		add_definitions(-DGENIE_MAJOR_VERSION=3)
		set(GENIE_REWEIGHT "$ENV{GENIE_REWEIGHT}")
		if(GENIE_REWEIGHT STREQUAL "")
			message(FATAL_ERROR "GENIE requested, but $GENIE_REWEIGHT is not set.  GENIE's reweight product required in v3 and above.")
			return()
		endif()
		set(NOVARWGT_GENIE_QUAL "genie3")
	else()
		add_definitions(-DGENIE_MAJOR_VERSION=2)
		set(NOVARWGT_GENIE_QUAL "genie2")
	endif()

	message(STATUS "Using GENIE version ${GENIE_VERSION} include directory at: ${GENIE}/src")
	include_directories(${GENIE}/src)
	if(genie_v3)
		include_directories(${GENIE_REWEIGHT}/src)
	endif()
	set(GENIE_LIB "$ENV{GENIE_LIB}")
	if(GENIE_LIB)
		set(GENIE_LIBDIR "${GENIE_LIB}")
	else()
		if(EXISTS "${GENIE}/lib")
			set(GENIE_LIBDIR "${GENIE}/lib")
		else()
			message(FATAL_ERROR "$GENIE set but cannot locate library location.  Please set $GENIE_LIB.")
			return()
		endif()
	endif()
else()
	message(STATUS "$GENIE not set; GENIE support disabled.")
endif()

# GENIE doesn't correctly link its libraries to ROOT or to each other,
# so we have to do it ourselves. :(
if(GENIE_LIBDIR)
	# GENIE depends on log4cpp.
	# it's possible that we may already have it around
	# (FNAL UPS), but if not, pkg-config can hopefully find it
	set(LOG4CPP_LIB "$ENV{LOG4CPP_LIB}")
	set(LOG4CPP_INC "$ENV{LOG4CPP_INC}")
	if(LOG4CPP_LIB AND LOG4CPP_INC)
		find_library(LOG4CPP_LIBRARIES log4cpp PATHS ENV LOG4CPP_LIB)
		include_directories(${LOG4CPP_INC})
	else()
		find_package(PkgConfig REQUIRED)
		pkg_check_modules(LOG4CPP REQUIRED log4cpp)
		include_directories(${LOG4CPP_INCLUDE_DIRS})
	endif()
	message(STATUS "Using log4cpp from: ${LOG4CPP_LIBRARIES}")

	# GENIE also depends on a few packages that fortunately cmake knows about
	find_package(LibXml2 REQUIRED)
	find_package(GSL REQUIRED)

	# last dependency: Pythia.
	# This one is typically provided as a user library, so the normal CMake/PkgConfig tools won't work.
	# GENIE users typically provide its location as an environment variable...
	# see the top of this script
	find_library(PYTHIA6_LIBRARIES Pythia6 PATHS "${PYTHIA6_LIBDIR}")
	if(NOT PYTHIA6_LIBRARIES)
		message(WARNING "$PYTHIA6_LIBRARIES defined but unabled to find Pythia6 library.  Linking may fail...")
	endif()
	add_library(Pythia6 SHARED IMPORTED)
	set_property(TARGET Pythia6 PROPERTY IMPORTED_LOCATION ${PYTHIA6_LIBRARIES})
	message(STATUS "Using Pythia6 from: ${PYTHIA6_LIBRARIES}")

	if(genie_v3)
		set(GENIE_LIB_LIST GFwMsg GFwReg GFwAlg GFwInt GFwGHEP GFwNum GFwUtl GFwEG GFwParDat
				           GPhXSIg GPhNuclSt GPhCmn GPhDcy GPhQELXS GPhHadTransp GRwFwk GRwClc)
	else()
		set(GENIE_LIB_LIST GMessenger GRegistry GAlgorithm GInteraction GHEP GBase
		                   GNumerical GUtils GPDG GBaryonResonance GEVGCore GEVGDrivers
				           GElFF GDecay GNuclear GLlewellynSmith GEVGModules GHadronTransp GReWeight)
	endif()
	foreach(LIB ${GENIE_LIB_LIST})
		unset(libloc CACHE)
		find_library(libloc ${LIB} PATHS ${GENIE_LIBDIR})
		if(NOT libloc)
			message(FATAL_ERROR "Could not find GENIE library: ${LIB}")
		endif()
		list(APPEND GENIE_LIBRARIES ${libloc})
	endforeach()
	message(STATUS "GENIE lib list: ${GENIE_LIBRARIES}")

	function(link_genie target)
	#message(STATUS "library list to add: ${liblist}")
		unset(libs CACHE)
		list(APPEND libs ${LOG4CPP_LIBRARIES})
		list(APPEND libs ${LIBXML2_LIBRARIES})
		list(APPEND libs ${GSL_LIBRARIES})
		list(APPEND libs ${PYTHIA6_LIBRARIES})
		list(APPEND libs ${GENIE_LIBRARIES})
		foreach(lib ${libs})
			target_link_libraries(${target} PUBLIC ${lib})
		endforeach()
	endfunction()

endif()
