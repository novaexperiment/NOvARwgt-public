install(DIRECTORY ./ DESTINATION inc
        PATTERN CMakeLists.txt EXCLUDE
        REGEX \.svn EXCLUDE
        REGEX \.git EXCLUDE)

