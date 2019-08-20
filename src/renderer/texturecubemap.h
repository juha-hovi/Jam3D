#pragma once

#include "texture.h"

namespace Jam3D {

class TextureCubeMap : public Texture
{
public:
    TextureCubeMap(int width, int height);
    void Bind(unsigned int slot = 0) const override;
	void Unbind() const override;
};

}