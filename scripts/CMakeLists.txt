if (NOVARWGT_INSTALL_UPS)
	install(DIRECTORY ./ DESTINATION scripts
	        PATTERN CMakeLists.txt EXCLUDE
			REGEX \.svn EXCLUDE
			REGEX \.git EXCLUDE)
endif()
