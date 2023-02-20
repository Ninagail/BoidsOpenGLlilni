#include "sphere_vertices.hpp"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <vector>
#include "glad/gl.h"

namespace glimac {

std::vector<ShapeVertex> sphere_vertices(float radius, GLsizei discLat, GLsizei discLong)
{
    // Équation paramétrique en (r, phi, theta) de la sphère
    // avec r >= 0, -PI / 2 <= theta <= PI / 2, 0 <= phi <= 2PI
    //
    // x(r, phi, theta) = r * sin(phi) * cos(theta)
    // y(r, phi, theta) = r * sin(theta) *
    // z(r, phi, theta) = r * cos(phi) * cos(theta)
    //
    // Discrétisation:
    // dPhi = 2PI / discLat, dTheta = PI / discLong
    //
    // x(r, i, j) = r * sin(i * dPhi) * cos(-PI / 2 + j * dTheta)
    // y(r, i, j) = r * sin(-PI / 2 + j * dTheta)
    // z(r, i, j) = r * cos(i * dPhi) * cos(-PI / 2 + j * dTheta)
    //
    // Attention ! dans cette implantation on duplique beaucoup de sommets. Une meilleur stratégie est de passer
    // par un Index Buffer Object, que nous verrons dans les prochains TDs

    const float rcpLat  = 1.f / discLat;
    const float rcpLong = 1.f / discLong;
    const float dPhi    = 2.f * glm::pi<float>() * rcpLat;
    const float dTheta  = glm::pi<float>() * rcpLong;

    std::vector<ShapeVertex> data;

    // Construit l'ensemble des vertex
    for (GLsizei j = 0; j <= discLong; ++j)
    {
        const GLfloat cosTheta = cos(-glm::pi<float>() / 2 + j * dTheta);
        const GLfloat sinTheta = sin(-glm::pi<float>() / 2 + j * dTheta);

        for (GLsizei i = 0; i <= discLat; ++i)
        {
            ShapeVertex vertex{};

            vertex.texCoords.x = i * rcpLat;
            vertex.texCoords.y = 1.f - j * rcpLong;

            vertex.normal.x = sin(i * dPhi) * cosTheta;
            vertex.normal.y = sinTheta;
            vertex.normal.z = cos(i * dPhi) * cosTheta;

            vertex.position = radius * vertex.normal;

            data.push_back(vertex);
        }
    }

    std::vector<ShapeVertex> vertices{};
    // Construit les vertex finaux en regroupant les données en triangles:
    // Pour une longitude donnée, les deux triangles formant une face sont de la forme:
    // (i, i + 1, i + discLat + 1), (i, i + discLat + 1, i + discLat)
    // avec i sur la bande correspondant à la longitude
    for (GLsizei j = 0; j < discLong; ++j)
    {
        const GLsizei offset = j * (discLat + 1);
        for (GLsizei i = 0; i < discLat; ++i)
        {
            vertices.push_back(data[offset + i]);
            vertices.push_back(data[offset + (i + 1)]);
            vertices.push_back(data[offset + discLat + 1 + (i + 1)]);
            vertices.push_back(data[offset + i]);
            vertices.push_back(data[offset + discLat + 1 + (i + 1)]);
            vertices.push_back(data[offset + i + discLat + 1]);
        }
    }

    return vertices;
}

} // namespace glimac
