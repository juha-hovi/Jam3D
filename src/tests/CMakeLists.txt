set(tests_source
	testbox.cpp
    testbox.h
)
add_library(tests ${tests_source})
target_link_libraries(tests renderer shapes)