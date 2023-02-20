#pragma once

#include <vector>
#include "common.hpp"

namespace glimac {

// Représente une sphère discrétisée centrée en (0, 0, 0) (dans son repère local)
// Son axe vertical est (0, 1, 0) et ses axes transversaux sont (1, 0, 0) et (0, 0, 1)
class Sphere {
public:
    // Constructeur: alloue le tableau de données et construit les attributs des vertex
    Sphere(GLfloat radius, GLsizei discLat, GLsizei discLong);

    // Renvoie le pointeur vers les données
    const ShapeVertex* getDataPointer() const { return m_Vertices.data(); }

    // Renvoie le nombre de vertex
    GLsizei getVertexCount() const { return m_nVertexCount; }

private:
    std::vector<ShapeVertex> m_Vertices;
    GLsizei                  m_nVertexCount; // Nombre de sommets
};

} // namespace glimac