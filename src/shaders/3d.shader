#Vertex
#version 330 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_TexCoord;
layout(location = 2) in vec3 in_Normal;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_FragPos;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main() 
{
    gl_Position = u_Proj * u_View * u_Model * vec4(in_Position, 1.0);
    
    v_TexCoord = in_TexCoord;
    v_Normal = mat3(transpose(inverse(u_Model))) * in_Normal;
    v_FragPos = vec3(u_Model * vec4(in_Position, 1.0));
}

#Fragment
#version 330 core

struct LightSource
{
    vec3 lightPosition;
    vec3 lightColor;
    float lightIntensity;
};

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_FragPos;

uniform sampler2D u_Texture;
uniform LightSource u_LightSources[10];
uniform mat4 u_Model;
uniform mat4 u_View;
uniform int u_LightSourceCount;
uniform bool u_ApplyLighting;
uniform bool u_ApplyTexture;
uniform vec4 u_Color;
uniform float u_FarPlane;
uniform samplerCube u_DepthMap;

vec3 GetCameraToFragmentVec()
{
    mat4 viewModel = inverse(u_Model * u_View);
    vec3 cameraToFragmentVec = vec3(viewModel[3]);
    return cameraToFragmentVec;
}

float GetShadow(int index)
{
    vec3 fragToLight = v_FragPos - u_LightSources[index].lightPosition;
    float closestDepth = texture(u_DepthMap, fragToLight).r;
    closestDepth *= u_FarPlane;

    float currentDepth = length(fragToLight);
    float bias = 10;
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;



    return shadow;
}

vec3 GetLighting(int index)
{
    float ambientStrength = u_LightSources[index].lightIntensity * 0.2;
    float specularStrength = u_LightSources[index].lightIntensity * 0.5;

    vec3 ambient = ambientStrength * u_LightSources[index].lightColor;

    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(u_LightSources[index].lightPosition - v_FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_LightSources[index].lightColor;

    vec3 viewDir = normalize(GetCameraToFragmentVec());
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular = specularStrength * spec * u_LightSources[index].lightColor;

    float shadow = 0.0;
    if (index == 0)
        shadow = GetShadow(index);

    vec3 light = ambient + (1.0 - shadow) * (diffuse + specular);
    return light;
}

void main()
{
    vec3 light = vec3(0.0, 0.0, 0.0);
    if (u_ApplyLighting)
    {
        for (int i = 0; i < u_LightSourceCount; ++i)
        {
            light += GetLighting(i);
        }
    }
    else 
    {
        light = vec3(1.0, 1.0, 1.0);
    }

    if (u_ApplyTexture)
    {
        vec4 texColor = texture(u_Texture, v_TexCoord);    
        color = vec4(light, 1.0) * texColor;
    }
    else
    {
        color = vec4(light, 1.0) * u_Color;
    }
}