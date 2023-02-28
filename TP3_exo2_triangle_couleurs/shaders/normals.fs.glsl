#version 330

in vec3 vNormals_vs;
in vec2 vTexCoords;

out vec4 fFragColor;
in float      depth;
uniform float fade;
void main()
{
    // fFragColor = vec4(vec3(depth / fade), 1.);
    fFragColor = vec4(normalize(vNormals_vs), 1.);
}