#pragma once

#include <vector>
#include "common.hpp"
#include "glad/gl.h"

namespace glimac {

// Représente une sphère discrétisée centrée en (0, 0, 0) (dans son repère local)
// Son axe vertical est (0, 1, 0) et ses axes transversaux sont (1, 0, 0) et (0, 0, 1)
class Sphere {
public:
    // Constructeur : alloue le tableau de données et construit les attributs des vertex
    Sphere(float radius, GLsizei discretisation_latitude, GLsizei discretisation_longitude);

    // Renvoie le pointeur vers les données
    const ShapeVertex* getDataPointer() const { return m_Vertices.data(); }

    // Renvoie le nombre de vertex
    GLsizei getVertexCount() const { return static_cast<GLsizei>(m_Vertices.size()); }

private:
    std::vector<ShapeVertex> m_Vertices;
};

} // namespace glimac