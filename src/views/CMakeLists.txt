set(views_source
    gridlayout.cpp
    gridlayout.h
    objectcreationview.cpp
    objectcreationview.h
	testview.cpp
    testview.h
    view.cpp
    view.h
)
add_library(views ${views_source})
target_link_libraries(views renderer shapes)