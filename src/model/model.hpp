#pragma once

#include <glimac/common.hpp>
#include "../src/scene/lights.hpp"
#include "../src/scene/loadShader.hpp"
#include "glm/fwd.hpp"
#include "p6/p6.h"

class Model {
protected:
    GLuint m_vbo;
    GLuint m_vao;
    // rendering::Texture m_texture; //pas encore géré
    std::vector<glimac::ShapeVertex> m_vertices;
    std::vector<int>                 m_index;
    GLsizei                          m_vertexCount;
    float                            m_Rotation;

public:
    /// \brief constructor param of the model class
    /// \param texture the texture of the model
    // Model(rendering::Texture texture):
    //     m_texture(texture)
    //     {}
    /// \brief default constructor
    Model() = default;

    /// \brief cdelete model
    ~Model()
    {
        glDeleteBuffers(1, &m_vbo);
        glDeleteVertexArrays(1, &m_vao);
    };

    /// \brief draw the model
    void drawArray(GLuint textName);

    /// \brief define the rotation of the model
    void setRotation(float angle);

    /// \brief set the drawSetup
    /// \param pos position of the model
    /// \param scale scale of the model
    /// \param ProjMatrix matrice of projection for the shader
    /// \param ViewMatrix viewMatrix (camera) for the shader
    /// \param program shader will be used for drawing the model
    /// \param textName identification of the Texture of the model
    void draw(glm::vec3 pos, glm::vec3 scales, glm::mat4 ProjMatrix, glm::mat4 ViewMatrix, LoadShader& loadShader, GLuint textName);

    /// \brief set and bind a vao
    void setVao();

    /// \brief set and bind vbo
    void setVbo();

    /// \brief set buffers
    void setBuffers();

    /// \brief get data
    std::vector<glimac::ShapeVertex> getData() const;

    /// \brief get data of the vertices
    const glimac::ShapeVertex* getDataPointer() const;

    /// \brief get the number of the vertices
    GLsizei getVertexCount() const
    {
        return m_vertexCount;
    }

    /// \brief get the index pointer of the model
    const int* getIndexPointer() const;

    /// \brief loader of .obj with TinyObjLoader librairy
    /// \param fileName name of the .obj
    void loadModel(const std::string& fileName);
};