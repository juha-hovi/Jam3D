#Vertex
#version 330 core

layout (location = 0) in vec3 in_Position;

uniform mat4 u_Model;

void main()
{
    gl_Position = u_Model * vec4(in_Position, 1.0);
}

#Geometry
#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

out vec4 g_FragmentPosition;

uniform mat4 u_ShadowMatrices[6];

void main()
{
    for (int side = 0; side < 6; ++side)
    {
        gl_Layer = side;
        for (int i = 0; i < 3; ++i)
        {
            g_FragmentPosition = gl_in[i].gl_Position;
            gl_Position = u_ShadowMatrices[side] * g_FragmentPosition;
            EmitVertex();
        }
        EndPrimitive();
    }
}

#Fragment
#version 330 core

in vec4 g_FragmentPosition;

uniform vec3 u_LightPosition;
uniform float u_FarPlane;

void main()
{
    float lightDistance = length(g_FragmentPosition.xyz - u_LightPosition);
    lightDistance = lightDistance / u_FarPlane;
    gl_FragDepth = lightDistance;
}