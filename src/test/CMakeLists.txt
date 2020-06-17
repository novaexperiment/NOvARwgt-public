add_executable(standalone_test
        ../../inc/NOvARwgt/test/tests_common.h
        standalone_test.cxx
)
list(APPEND TEST_TARGETS standalone_test)

add_executable(hash_test
        hash_test.cxx)
list(APPEND TEST_TARGETS hash_test)

if(USE_NUSIMDATA)
    add_executable(nutools_test
            ../../inc/NOvARwgt/test/tests_common.h
            nutools_test.cxx
    )
    link_nusimdata(nutools_test)
    list(APPEND TEST_TARGETS nutools_test)
endif()

if(USE_GENIE)
    include_directories(${LOG4CPP_INC})
    add_executable(genie_test
            ../../inc/NOvARwgt/test/tests_common.h
            genie_test.cxx
            )
    link_genie(genie_test)
    list(APPEND TEST_TARGETS genie_test)
endif()

#foreach(LIB ${ROOT_LIBRARIES})
#    target_link_libraries(standalone_test ${LIB})
#endforeach()
#
#if(GENIE_LIBDIR)
#    add_deps("${GENIE_LIB_LIST}" ${GENIE_LIBDIR} standalone_test)
#endif()

foreach(test_tgt ${TEST_TARGETS})
    foreach(LIB ${ROOT_LIBRARIES})
        #    message(STATUS "  added ROOT dependency ${LIB} to NOvARwgt")
        target_link_libraries(${test_tgt} PUBLIC ${LIB})
    endforeach()

    foreach(LIB ${CETLIB_LIBRARIES})
        message(STATUS "  added CETlib dependency ${LIB} to exec ${test_tgt}")
        target_link_libraries(${test_tgt} PRIVATE ${LIB})
    endforeach()

    target_link_libraries(${test_tgt} PUBLIC NOvARwgt)
endforeach()


install(TARGETS ${TEST_TARGETS} RUNTIME DESTINATION ${TARGET_BINDIR} PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                                                                                 GROUP_READ GROUP_EXECUTE
                                                                                 WORLD_READ WORLD_EXECUTE)
