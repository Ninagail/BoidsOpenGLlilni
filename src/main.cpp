#include <glm/gtc/matrix_transform.hpp>
#include "../src/boids/boids.hpp"
#include "glimac/common.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"
#include "tools.cpp"


int main()
{
    auto ctx = p6::Context{{1280, 720, "Boids"}};
    ctx.maximize_window();

    const p6::Shader shader = p6::load_shader(
        "shaders/3D.vs.glsl",
        "shaders/normals.fs.glsl"
    );

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/
    

    std::vector<glimac::ShapeVertex> vertices = glimac::sphere_vertices(1, 32, 16);

    VBO vbo;
    vbo.bind();
    vbo.setData(vertices.data(), vertices.size() * sizeof(glimac::ShapeVertex));
    vbo.unbind();

    VAO vao;
    vao.bind();

    static constexpr GLuint vertex_attr_position = 0;
    glEnableVertexAttribArray(vertex_attr_position);
    static constexpr GLuint vertex_attr_normal = 1;
    glEnableVertexAttribArray(vertex_attr_normal);
    static constexpr GLuint vertex_attr_texcoords = 2;
    glEnableVertexAttribArray(vertex_attr_texcoords);

    vbo.bind();
    glVertexAttribPointer(vertex_attr_position, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, position));
    glVertexAttribPointer(vertex_attr_normal, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, normal));
    glVertexAttribPointer(vertex_attr_texcoords, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, texCoords));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    vao.unbind();

    // shader.use();

    // GLint uMVMatrixLoc     = glGetUniformLocation(shader.id(), "uMVMatrix");
    // GLint uMVPMatrixLoc    = glGetUniformLocation(shader.id(), "uMVP");
    // GLint uNormalMatrixLoc = glGetUniformLocation(shader.id(), "uNormalMatrix");

    glEnable(GL_DEPTH_TEST);

    // Declare your infinite update loop.
    ctx.update = [&]() {
        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        vao.bind();
        shader.use();

        int uMVPMatrix    = glGetUniformLocation(shader.id(), "uMVPMatrix");
        int uMVMatrix     = glGetUniformLocation(shader.id(), "uMVMatrix");
        int uNormalMatrix = glGetUniformLocation(shader.id(), "uNormalMatrix");

        glm::mat4 ProjMatrix   = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        glm::mat4 MVMatrix     = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -5));
        glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        // Envoyez les matrices au GPU
        glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix));
        glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
        glDrawArrays(GL_TRIANGLES, 0, size(vertices));
        glBindVertexArray(0);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}
