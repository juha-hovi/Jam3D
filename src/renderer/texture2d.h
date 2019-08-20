#pragma once

#include "texture.h"

#include <string>

namespace Jam3D {

class Texture2D : public Texture 
{
public:
    Texture2D(const std::string& fp);
    void Bind(unsigned int slot = 0) const override;
	void Unbind() const override;

private:
    std::string m_FilePath;
	unsigned char* m_LocalBuffer;
    int m_BPP;
};
    
}