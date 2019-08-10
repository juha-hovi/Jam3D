set(renderer_source
	indexbuffer.cpp
	indexbuffer.h
	renderer.cpp
	renderer.h
	shader.cpp
	shader.h
	texture.cpp
	texture.h
	vertexarray.cpp
	vertexarray.h
	vertexbuffer.cpp
	vertexbuffer.h
	vertexbufferlayout.h
)
add_library(renderer ${renderer_source})
target_link_libraries(renderer stb_image)