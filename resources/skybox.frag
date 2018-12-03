#version 330 core
// Based on the skybox example from learnopengl.com

in vec3 TexCoords;
out vec4 color;

uniform samplerCube skybox;

void main()
{    
    color = texture(skybox, TexCoords);
}