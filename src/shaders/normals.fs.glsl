#version 330

in vec3 vPosition_vs;
in vec3 vNormal_vs;
in vec2 vTexCoord_vs;

out vec4 fFragColor;

void main()
{
    vec3 normal = normalize(vNormal_vs);
    fFragColor= vec4(vNormal_vs, 1.0);
}