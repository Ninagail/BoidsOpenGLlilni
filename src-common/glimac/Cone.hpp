#pragma once
#include <vector>
#include "common.hpp"
#include "glad/gl.h"

namespace glimac {
    
// Représente un cone ouvert discrétisé dont la base est centrée en (0, 0, 0) (dans son repère local)
// Son axe vertical est (0, 1, 0) et ses axes transversaux sont (1, 0, 0) et (0, 0, 1)
std::vector<ShapeVertex> cone_vertices(float height, float radius, GLsizei discretization_latitude, GLsizei discretization_height);

} // namespace glimac
