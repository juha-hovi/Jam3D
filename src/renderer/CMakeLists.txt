set(renderer_source
	camera.cpp
	camera.h
	orthocamera.cpp
	orthocamera.h
	perspectivecamera.cpp
	perspectivecamera.h
	framebuffer.cpp
	framebuffer.h
	indexbuffer.cpp
	indexbuffer.h
	renderer.cpp
	renderer.h
	shader.cpp
	shader.h
	texture.cpp
	texture.h
	texture2d.cpp
	texture2d.h
	texturecubemap.cpp
	texturecubemap.h
	vertexarray.cpp
	vertexarray.h
	vertexbuffer.cpp
	vertexbuffer.h
	vertexbufferlayout.h
	viewport.cpp
	viewport.h
)
add_library(renderer ${renderer_source})
target_link_libraries(renderer misc stb_image)