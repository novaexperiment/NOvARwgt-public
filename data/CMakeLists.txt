install(DIRECTORY ./ DESTINATION data
        PATTERN CMakeLists.txt EXCLUDE
        REGEX \.svn EXCLUDE
        REGEX \.git EXCLUDE)

