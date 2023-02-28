#version 330

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aVertexNormal;
layout(location = 3) in vec2 aVertexTexCoords;

uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;

uniform float offset;

out vec2 vTexCoords;
out vec3 vNormals_vs;
out float depth;

void main()
{
    vTexCoords  = aVertexTexCoords;
    vec4 normal = /* uNormalMatrix * */ vec4(aVertexNormal, 0.);
    vNormals_vs = normal.xyz;
    vec4 pos    = uMVPMatrix * vec4(aVertexPosition + vec3(0, 0, offset), 1.);
    gl_Position = pos;
    depth       = pos.z / pos.w;
}