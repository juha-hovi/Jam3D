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
    vec3 lightPos;
    vec3 lightColor;
    float ambientStrength;
    float specularStrength;
};

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_FragPos;

uniform sampler2D u_Texture;
uniform LightSource u_LightSources[2];
uniform mat4 u_Model;
uniform mat4 u_View;

vec3 GetCameraToFragmentVec()
{
    mat4 viewModel = inverse(u_Model * u_View);
    vec3 cameraToFragmentVec = vec3(viewModel[3]);
    return cameraToFragmentVec;
}

vec3 GetLighting(int index)
{
    vec3 ambient = u_LightSources[index].ambientStrength * u_LightSources[index].lightColor;

    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(u_LightSources[index].lightPos - v_FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_LightSources[index].lightColor;

    vec3 viewDir = normalize(GetCameraToFragmentVec());
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular = u_LightSources[index].specularStrength * spec * u_LightSources[index].lightColor;

    vec3 light = ambient + diffuse + specular;
    return light;
}

void main()
{
    vec3 light0 = GetLighting(0);
    vec3 light1 = GetLighting(1);

    vec4 texColor = texture(u_Texture, v_TexCoord);
    
    color = vec4(light0 + light1, 1.0) * texColor;
}