// Vertex Shader
#version 330 core

layout (location = 0) in vec3 aPosition; // Vertex position
layout (location = 1) in vec3 aNormal;   // Vertex normal
layout (location = 2) in vec2 aTexCoord; // Texture coordinates

uniform mat4 u_model;      // Model matrix
uniform mat4 u_view;       // View matrix
uniform mat4 u_proj; // Projection matrix

out vec3 vFragPos;       // Fragment position in world space
out vec3 vNormal;        // Normal vector in world space
out vec2 vTexCoord;      // Pass texture coordinates to fragment shader

void main()
{
    vFragPos = vec3(u_model * vec4(aPosition, 1.0));
    vNormal = aNormal;
    vTexCoord = aTexCoord;

    gl_Position = u_proj * u_view * vec4(vFragPos, 1.0);
}
