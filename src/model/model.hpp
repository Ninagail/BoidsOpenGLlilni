#pragma once

#include <glm/glm.hpp>
#include "../src/scene/loadShader.hpp"
#include "glm/fwd.hpp"
#include "p6/p6.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

int const window_width  = 1280;
int const window_height = 720;

bool loadOBJ(const std::string& objPath, std::vector<glimac::ShapeVertex>& vertices)
{
    std::string                      inputfile = objPath;
    tinyobj::attrib_t                attrib;
    std::vector<tinyobj::shape_t>    shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn;
    std::string err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, inputfile.c_str(), nullptr);

    if (!warn.empty())
    {
        std::cout << warn << std::endl;
    }

    if (!err.empty())
    {
        std::cerr << err << std::endl;
    }

    if (!ret)
    {
        exit(1);
    }

    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++)
    {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
        {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++)
            {
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

                glimac::ShapeVertex newVertex = glimac::ShapeVertex(

                    // POSITION
                    glm::vec3(
                        tinyobj::real_t(attrib.vertices[3 * size_t(idx.vertex_index) + 0]),
                        tinyobj::real_t(attrib.vertices[3 * size_t(idx.vertex_index) + 1]),
                        tinyobj::real_t(attrib.vertices[3 * size_t(idx.vertex_index) + 2])
                    ),

                    // NORMAL
                    glm::vec3(
                        tinyobj::real_t(attrib.normals[3 * size_t(idx.normal_index) + 0]), // nx
                        tinyobj::real_t(attrib.normals[3 * size_t(idx.normal_index) + 1]), // ny
                        tinyobj::real_t(attrib.normals[3 * size_t(idx.normal_index) + 2])  // nz
                    ),

                    // TEXTURE_COORDINATES
                    glm::vec2(
                        tinyobj::real_t(attrib.texcoords[2 * size_t(idx.texcoord_index) + 0]), // tx
                        tinyobj::real_t(attrib.texcoords[2 * size_t(idx.texcoord_index) + 1])  // ty
                    )
                );
                vertices.push_back(newVertex);
                // Optional: vertex colors
                // tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
                // tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
                // tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
            }
            index_offset += fv;

            // per-face material
            shapes[s].mesh.material_ids[f];
        }
    }
};