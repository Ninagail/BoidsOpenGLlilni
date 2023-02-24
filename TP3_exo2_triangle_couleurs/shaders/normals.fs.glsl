#version 330

in vec3 vNormals_vs;

out vec4 fFragColor;

void main()
{
    fFragColor = vec4(normalize(vNormals_vs), 1.);
}