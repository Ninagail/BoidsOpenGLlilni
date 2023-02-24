#version 330

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aVertexNormal;
layout(location = 2) in vec2 aVertexTexCoords;

uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;

out vec2 vTexCoords;
out vec3 vNormals_vs;

void main()
{
    vTexCoords  = aVertexTexCoords;
    vec4 normal = uNormalMatrix * vec4(aVertexNormal, 1.);
    vNormals_vs = normal.xyz / normal.w;
    gl_Position = uMVPMatrix * vec4(aVertexPosition, 1.);
}