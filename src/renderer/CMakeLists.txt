set(renderer_source
	indexbuffer.cpp
	indexbuffer.h
	renderer.cpp
	renderer.h
	shader.cpp
	shader.h
	vertexarray.cpp
	vertexarray.h
	vertexbuffer.cpp
	vertexbuffer.h
	vertexbufferlayout.h
)
add_library(renderer $(renderer_source))