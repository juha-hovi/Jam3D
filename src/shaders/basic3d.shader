#Vertex
#version 330 core

layout(location = 0) in vec4 in_Position;
layout(location = 1) in vec2 in_TexCoord;
layout(location = 2) in vec4 in_Normal;

out vec2 v_TexCoord;
out vec4 v_Normal;

uniform mat4 u_MVP;

void main() 
{
    gl_Position = u_MVP * in_Position;
    v_TexCoord = in_TexCoord;
    v_Normal = in_Normal;
}

#Fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 v_Normal;

uniform sampler2D u_Texture;

void main()
{
    vec4 texColor = texture(u_Texture, v_TexCoord);
    color = texColor;
}