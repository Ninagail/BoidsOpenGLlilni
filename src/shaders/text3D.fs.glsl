#version 330

//variables d'entrées
in vec3 vPosition_vs;
in vec3 vNormal_vs;
in vec2 vTexCoords;

vec2 fCorrectCoords;

out vec4 fFragColor;

uniform sampler2D uTextLadybug;


// uniform sampler2D uTexture1;

void main() {

    fCorrectCoords = vec2(vTexCoords.x, 1-vTexCoords.y); //pour le template p6
	// vec4 normaleNormalise = vec4(normalize(vNormal_vs), 1.);

	fFragColor = texture(uTextLadybug, fCorrectCoords);
	
    // fFragColor = texture(uTexture1, fCorrectCoords);
}