// #version 330

// in vec3 vPosition_vs;
// in vec3 vNormal_vs;
// in vec2 vTexCoord_vs;

// out vec4 fFragColor;

// void main()
// {
//     vec3 normal = normalize(vNormal_vs);
//     fFragColor= vec4(vNormal_vs, 1.0);
// }

#version 330

// Déclaration de l'uniforme pour la couleur
uniform vec3 uColor;

in vec3 vPosition_vs; // Position modifiée par le vertex shader
in vec3 vNormal_vs;   // Normale modifiée par le vertex shader
in vec2 vTexCoords;   // Coordonnées de texture

out vec4 fragColor; // Couleur finale du fragment

void main() {
    // Utilisation de la couleur uniforme
    vec3 finalColor = uColor;

    // Ajustement de la couleur en fonction de la normale
    finalColor *= abs(dot(normalize(vNormal_vs), normalize(vec3(1.0, 1.0, 1.0))));

    // Output de la couleur finale
    fragColor = vec4(finalColor, 1.0);
}
