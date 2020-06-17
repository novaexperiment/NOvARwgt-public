set(HEADER_FILES
		../inc/NOvARwgt/util/GeneratorSupportConfig.h
        ../inc/NOvARwgt/util/HistWrapper.h
		../inc/NOvARwgt/util/InputVals.h
		../inc/NOvARwgt/util/ITestGenVersion.h
        ../inc/NOvARwgt/util/LazyROOTObjLoader.h
		../inc/NOvARwgt/util/Registry.h

        ../inc/NOvARwgt/rwgt/EventRecord.h
		../inc/NOvARwgt/rwgt/IWeightGenerator.h
		../inc/NOvARwgt/rwgt/ISystKnob.h

		../inc/NOvARwgt/rwgt/generic/NueNumuSysts.h

		../inc/NOvARwgt/rwgt/genie/GenieKnobNames.h
		../inc/NOvARwgt/rwgt/genie/GenieSystKnob.h

		../inc/NOvARwgt/rwgt/genie/COH/COHSysts.h

		../inc/NOvARwgt/rwgt/genie/DIS/HighWDISWeight.h
        ../inc/NOvARwgt/rwgt/genie/DIS/Nonres1piWeights.h

		../inc/NOvARwgt/rwgt/genie/DIS/NonresPiSysts.h

        ../inc/NOvARwgt/rwgt/genie/MEC/EmpiricalMECFixups.h
        ../inc/NOvARwgt/rwgt/genie/MEC/EmpiricalMECTuneBase.h
        ../inc/NOvARwgt/rwgt/genie/MEC/EmpiricalMECTuneSA.h
        ../inc/NOvARwgt/rwgt/genie/MEC/EmpiricalMECTune2017.h
        ../inc/NOvARwgt/rwgt/genie/MEC/EmpiricalMECTune2018.h
        ../inc/NOvARwgt/rwgt/genie/MEC/EmpiricalMECOtherTunes.h

		../inc/NOvARwgt/rwgt/genie/MEC/EmpiricalMECSystsSA.h
		../inc/NOvARwgt/rwgt/genie/MEC/EmpiricalMECSysts2017.h
		../inc/NOvARwgt/rwgt/genie/MEC/EmpiricalMECSysts2018.h

		../inc/NOvARwgt/rwgt/genie/QE/MAQEWgts.h
        ../inc/NOvARwgt/rwgt/genie/QE/RPAWeights.h

		../inc/NOvARwgt/rwgt/genie/QE/MAQESysts.h
		../inc/NOvARwgt/rwgt/genie/QE/RPASysts.h

        ../inc/NOvARwgt/rwgt/Tune.h

	    ../inc/NOvARwgt/rwgt/tunes/TunesSA.h
	    ../inc/NOvARwgt/rwgt/tunes/Tunes2017.h
		../inc/NOvARwgt/rwgt/tunes/Tunes2018.h

		rwgt/genie/GenieInternalTools.h
)

set(SOURCES
	util/GeneratorSupportConfig.cxx
    util/HistWrapper.cxx
	util/InputVals.cxx
    util/LazyROOTObjLoader.cxx
	util/Registry.cxx

	rwgt/generic/NueNumuSysts.cxx

	rwgt/genie/GenieInternalTools.cxx
	rwgt/genie/GenieSystKnob.cxx

	rwgt/genie/COH/COHSysts.cxx

    rwgt/genie/DIS/HighWDISWeight.cxx
    rwgt/genie/DIS/Nonres1piWeights.cxx

	rwgt/genie/DIS/NonresPiSysts.cxx

    rwgt/genie/MEC/EmpiricalMECFixups.cxx
    rwgt/genie/MEC/EmpiricalMECTuneBase.cxx
    rwgt/genie/MEC/EmpiricalMECTuneSA.cxx
    rwgt/genie/MEC/EmpiricalMECTune2017.cxx
	rwgt/genie/MEC/EmpiricalMECTune2018.cxx
    rwgt/genie/MEC/EmpiricalMECOtherTunes.cxx

	rwgt/genie/MEC/EmpiricalMECSystsSA.cxx
	rwgt/genie/MEC/EmpiricalMECSysts2017.cxx
	rwgt/genie/MEC/EmpiricalMECSysts2018.cxx

	rwgt/genie/QE/MAQEWgts.cxx
    rwgt/genie/QE/RPAWeights.cxx

	rwgt/genie/QE/MAQESysts.cxx
	rwgt/genie/QE/RPASysts.cxx

	rwgt/tunes/TunesSA.cxx
	rwgt/tunes/Tunes2017.cxx
	rwgt/tunes/Tunes2018.cxx

    rwgt/EventRecord.cxx
    rwgt/Tune.cxx
)

if(USE_GENIE)
	set(HEADER_FILES ${HEADER_FILES}  ../inc/NOvARwgt/interfaces/GenieInterface.h)
	set(SOURCES      ${SOURCES}       interfaces/GenieInterface.cxx)
endif()
if(USE_NUSIMDATA)
	set(HEADER_FILES ${HEADER_FILES}  ../inc/NOvARwgt/interfaces/NuToolsInterface.h)
	set(SOURCES      ${SOURCES}       interfaces/NuToolsInterface.cxx)
endif()

add_library(NOvARwgt SHARED
        ${HEADER_FILES}
		${SOURCES}
 )

#string(REPLACE " " ";" "${ROOT_LIB_NAMES}" root_libs)
#separate_arguments(root_libs "${ROOT_LIB_NAMES}")
foreach(LIB ${ROOT_LIBRARIES})
#    message(STATUS "  added ROOT dependency ${LIB} to NOvARwgt")
    target_link_libraries(NOvARwgt PUBLIC ${LIB})
endforeach()

foreach(LIB ${CETLIB_LIBRARIES})
#    message(STATUS "  added CETlib dependency ${LIB} to NOvARwgt")
	target_link_libraries(NOvARwgt PRIVATE ${LIB})
endforeach()


if(USE_GENIE)
	link_genie(NOvARwgt)
endif()
if(USE_NUSIMDATA)
	link_nusimdata(NOvARwgt)
endif()

target_compile_options(NOvARwgt PRIVATE -Wall -Wextra -pedantic)

install(TARGETS NOvARwgt LIBRARY DESTINATION ${TARGET_LIBDIR})

if(NOVARWGT_INSTALL_SOURCE)
	install(DIRECTORY ./ DESTINATION src
			PATTERN CMakeLists.txt EXCLUDE
			REGEX \.svn EXCLUDE
			REGEX \.git EXCLUDE)
endif()