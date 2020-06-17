if (NOVARWGT_INSTALL_UPS)
	install(DIRECTORY ./ DESTINATION ups
	        PATTERN CMakeLists.txt EXCLUDE
		REGEX \.svn EXCLUDE
		REGEX \.git EXCLUDE) 
endif()
