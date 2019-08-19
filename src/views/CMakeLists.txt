set(views_source
	testview.cpp
    testview.h
)
add_library(views ${views_source})
target_link_libraries(views math renderer shapes)