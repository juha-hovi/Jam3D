#Vertex
#version 330 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_TexCoord;
layout(location = 2) in vec3 in_Normal;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_FragPos;
out vec3 v_CameraPos;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main() 
{
    gl_Position = u_Proj * u_View * u_Model * vec4(in_Position, 1.0);
    
    v_TexCoord = in_TexCoord;
    v_Normal = in_Normal;
    v_FragPos = vec3(u_Model * vec4(in_Position, 1.0) * u_Model);
    
    mat3 rotMat = mat3(u_View * u_Model);
    vec3 d = vec3((u_View * u_Model)[3]);
    v_CameraPos = -d * rotMat;
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
in vec3 v_CameraPos;

uniform sampler2D u_Texture;
uniform LightSource u_LightSource;

void main()
{
    vec3 ambient = u_LightSource.ambientStrength * u_LightSource.lightColor;

    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(u_LightSource.lightPos - v_FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_LightSource.lightColor;

    vec3 viewDir = normalize(v_CameraPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular = u_LightSource.specularStrength * spec * u_LightSource.lightColor;

    vec3 light = ambient + diffuse + specular;
    vec4 texColor = texture(u_Texture, v_TexCoord);
    
    color = vec4(light, 1.0) * texColor;
}