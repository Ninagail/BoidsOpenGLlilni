#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

class Model {
public:
    struct Vertex {
        glm::vec3 position;
        glm::vec2 texCoord;
        glm::vec3 normal;
    };

    Model(const std::string& filename);

    const std::vector<Vertex>& getVertices() const;

private:
    std::vector<Vertex> m_vertices;

    bool loadObj(const std::string& filename);
};
