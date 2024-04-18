#include "../src/model/model.hpp"
#include <glimac/common.hpp>
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/matrix.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

void Model::drawArray(GLuint textName)
{
    glBindVertexArray(m_vao);
    glBindTexture(GL_TEXTURE_2D, textName);
    // if(m_isIbo){
    //     glDrawElements(GL_TRIANGLES, getVertexCount(), GL_UNSIGNED_INT,0);
    // }
    // else{
    glDrawArrays(GL_TRIANGLES, 0, getVertexCount());
    // }
}

void Model::draw(glm::vec3 pos, glm::vec3 scale, glm::mat4 ProjMatrix, glm::mat4 ViewMatrix, LoadShader& loadShader, GLuint textName)
{
    glm::mat4 ViewMatrixModel = glm::translate(glm::mat4(1.0), pos);
    ViewMatrixModel           = glm::scale(ViewMatrixModel, scale);
    ViewMatrixModel           = ViewMatrix * ViewMatrixModel;

    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(ViewMatrixModel));
    loadShader.setUniform1i("uTextLadybug", 0);
    loadShader.setUniform4fv("uMVPMatrix", ProjMatrix * ViewMatrixModel);
    loadShader.setUniform4fv("uMVMatrix", ViewMatrixModel);
    loadShader.setUniform4fv("uNormalMatrix", NormalMatrix);

    // les params de lights sont définies avant

    this->drawArray(textName);
}

void Model::setVbo()
{
    GLuint vbo = 0;
    m_vbo      = vbo;
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, getVertexCount() * sizeof(glimac::ShapeVertex), getDataPointer(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Model::setRotation(float angle)
{
    angle = glm::mod(angle, glm::two_pi<float>());

    m_Rotation = angle;
}

void Model::setVao()
{
    GLuint vao = 0;
    m_vao      = vao;
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    // if(m_isIbo){
    //     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    // }
    const GLuint VERTEX_ATTR_POSITION  = 0;
    const GLuint VERTEX_ATTR_NORMAL    = 1;
    const GLuint VERTEX_ATTR_TEXCOORDS = 2;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, position));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, normal));
    glVertexAttribPointer(VERTEX_ATTR_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, texCoords));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Model::setBuffers()
{
    setVbo();
    setVao();
}

std::vector<glimac::ShapeVertex> Model::getData() const
{
    return m_vertices;
}

const glimac::ShapeVertex* Model::getDataPointer() const
{
    return m_vertices.data();
}

const int* Model::getIndexPointer() const
{
    return &m_index[0];
}

void Model::loadModel(const std::string& fileName)
{
    // Load 3D object
    std::string                      inputfile = "assets/models/" + fileName;
    tinyobj::attrib_t                attrib;
    std::vector<tinyobj::shape_t>    shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn;
    std::string err;
    bool        ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, inputfile.c_str(), nullptr);

    if (!err.empty())
    { // `err` may contain warning message.
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
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                // access to vertex
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

                m_vertices.push_back(newVertex);
            }
            index_offset += fv;
        }
    }
    m_vertexCount = m_vertices.size();
}
