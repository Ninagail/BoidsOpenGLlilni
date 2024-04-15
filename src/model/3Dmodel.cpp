#include <fstream>
#include <iostream>
#include <sstream>
#include "3Dmodel.hpp"


Model::Model(const std::string& filename)
{
    if (!loadObj(filename))
    {
        std::cerr << "Error: Failed to load model " << filename << std::endl;
    }
}

const std::vector<Model::Vertex>& Model::getVertices() const
{
    return m_vertices;
}

bool Model::loadObj(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: Failed to open file " << filename << std::endl;
        return false;
    }

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string        type;
        iss >> type;
        if (type == "v")
        {
            glm::vec3 position;
            iss >> position.x >> position.y >> position.z;
            positions.push_back(position);
        }
        else if (type == "vt")
        {
            glm::vec2 texCoord;
            iss >> texCoord.x >> texCoord.y;
            texCoords.push_back(texCoord);
        }
        else if (type == "vn")
        {
            glm::vec3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        }
        else if (type == "f")
        {
            std::string vertex;
            while (iss >> vertex)
            {
                std::istringstream viss(vertex);
                std::string        indexStr;
                std::getline(viss, indexStr, '/');
                int posIndex = std::stoi(indexStr) - 1;
                std::getline(viss, indexStr, '/');
                int texCoordIndex = std::stoi(indexStr) - 1;
                std::getline(viss, indexStr, '/');
                int    normalIndex = std::stoi(indexStr) - 1;
                Vertex vert;
                vert.position = positions[posIndex];
                vert.texCoord = texCoords[texCoordIndex];
                vert.normal   = normals[normalIndex];
                m_vertices.push_back(vert);
            }
        }
    }

    file.close();

    return true;
}
