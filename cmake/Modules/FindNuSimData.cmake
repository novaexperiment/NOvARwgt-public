####  FindNuSimData.cmake:
####     Locate the NuSimData package via the Fermilab UPS system.
####      (NuSimData itself has a number of dependencies which are best resolved via UPS.)
####     This requires a lot of shenanigans because the CMake UPS interface assumes
####     you want to build your package using CET as well, and we don't.
####
####   Original author:  J. Wolcott <jwolcott@fnal.gov>
####   Date:             Oct. 29, 2018

find_package(CET)

find_ups_product(nusimdata)

set(NUSIMDATA_INC $ENV{NUSIMDATA_INC})
set(NUSIMDATA_LIBDIR $ENV{NUSIMDATA_LIB})

set(NUSIMDATA_LIB_LIST nusimdata_SimulationBase)

if(NUSIMDATA_INC AND NUSIMDATA_LIBDIR)
    if(${NUSIMDATA_VERSION} STRLESS "v1_16_04" )
        message(WARNING "Found nusimdata version ${NUSIMDATA_VERSION}, but require at least v_16_04.  Not building nutools interface.")
        set(USE_NUSIMDATA FALSE)
    else()
        message(STATUS "searching for NuSimData libraries in dir: ${NUSIMDATA_LIBDIR}")
        set(USE_NUSIMDATA TRUE)
    endif()
endif()

if(USE_NUSIMDATA)
    foreach(LIB ${NUSIMDATA_LIB_LIST})
        add_library(${LIB} SHARED IMPORTED)
        unset(libdir)
        unset(libdir CACHE)
        find_library(libdir ${LIB} PATHS "${NUSIMDATA_LIBDIR}")
#        message(STATUS "  found NuSimData library: ${LIB} at ${libdir}")
        set_property(TARGET ${LIB} PROPERTY IMPORTED_LOCATION ${libdir})
        set_property(TARGET ${LIB} PROPERTY INTERFACE_LINK_LIBRARIES ${dependency_libs})
    endforeach()

    set(NUSIMDATA_LIBDIR "$ENV{NUSIMDATA_LIB}")
    include_directories(${NUSIMDATA_INC})
    set(NUSIMDATA_LIB_LIST nusimdata_SimulationBase)

    function(link_nusimdata target)
        foreach(lib ${NUSIMDATA_LIB_LIST})
            target_link_libraries(${target} PRIVATE ${lib})
        endforeach()
        #    add_deps("${NUSIMDATA_LIB_LIST}" ${NUSIMDATA_LIBDIR} NOvARwgt)
    endfunction()
else()
    message(STATUS "Missing either $NUSIMDATA_INC or $NUSIMDATA_LIB.  NuSimData extensions disabled.")
endif()

