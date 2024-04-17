#include <iostream>
#include <vector>
#include "../src/tools/vbovao.hpp"
#include "boids/boids.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/random.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "img/src/Image.h"
#include "p6/p6.h"

int const window_width  = 1280;
int const window_height = 720;

int main()
{
    auto ctx = p6::Context{{window_width, window_height, "TP3 EX1"}};
    ctx.maximize_window();

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    std::vector<Boids> boids(50);
    for (auto& boid : boids)
    {
        boid.set_speed();
        boid.set_position();
    }
    // Load shaders
    p6::Shader Shader = p6::load_shader("shaders/3D.vs.glsl", "shaders/text3D.fs.glsl");

    // Load texture
    img::Image img_ladybug = p6::load_image_buffer("assets/models/Ladybug.jpg");

    // Retrieve uniform variables
    GLint uMVPMatrix    = glGetUniformLocation(Shader.id(), "uMVPMatrix");
    GLint uMVMatrix     = glGetUniformLocation(Shader.id(), "uMVMatrix");
    GLint uNormalMatrix = glGetUniformLocation(Shader.id(), "uNormalMatrix");
    GLint uTextLadybug  = glGetUniformLocation(Shader.id(), "uTextLadybug");

    // Creation du VBO
    VBO vbo;
    vbo.bind();

    // Creation de la forme de la sphère
    const std::vector<glimac::ShapeVertex> vertices = glimac::sphere_vertices(1.f, 32, 16);

    // Fill coordinates in the VBO (Static is for constant variables)
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glimac::ShapeVertex), vertices.data(), GL_STATIC_DRAW);

    // Option to see tests in depth?
    glEnable(GL_DEPTH_TEST);

    // Initialisation de la texture
    GLuint textureLadybug;
    glGenTextures(1, &textureLadybug);
    glBindTexture(GL_TEXTURE_2D, textureLadybug);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_ladybug.width(), img_ladybug.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img_ladybug.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Creation du VAO
    VAO vao;
    vao.bind();

    const GLuint VERTEX_ATTR_POSITION = 0;
    const GLuint VERTEX_ATTR_NORM     = 1;
    const GLuint VERTEX_ATTR_UV       = 2;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORM);
    glEnableVertexAttribArray(VERTEX_ATTR_UV);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, position)));
    glVertexAttribPointer(VERTEX_ATTR_NORM, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, normal)));
    glVertexAttribPointer(VERTEX_ATTR_UV, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, texCoords)));

    // Debind du VBO
    vbo.unbind();

    // Debind du VAO
    vao.unbind();

    // Calcul des différentes matrices nécessaires pour les shaders
    glm::mat4 ProjMatrix;
    glm::mat4 MVMatrix;
    glm::mat4 NormalMatrix;

    ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);

    /* Loop until the user closes the window */
    ctx.update = [&]() {
        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/
        Shader.use();
        glClearColor(0.2f, 0.2f, 0.2f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Bind VAO
        vao.bind();

        // Bind texture Ladybug
        glBindTexture(GL_TEXTURE_2D, textureLadybug);

        // Ladybug
        MVMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0., 0., -5.));
        // MVMatrix     = glm::rotate(MVMatrix, glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
        NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        // Set texture Ladybug
        glUniform1i(uTextLadybug, 0);

        // Apply matrices in the shaders
        glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
        glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(MVMatrix))));

        // Draw the vertices
        // glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        ImGui::Begin("Slider");
        ImGui::SliderFloat("CohesionForce", &Boids::cohesion_force, 0.f, 1.f);
        ImGui::SliderFloat("SeparationForce", &Boids::separation_force, 0.f, 1.f);
        ImGui::SliderFloat("Distance to unite", &Boids::distance_cohesion, 0.f, 1.f);
        ImGui::SliderFloat("Distance to escape", &Boids::distance_separation, 0.f, 1.f);
        ImGui::SliderFloat("Distance to align", &Boids::distance_alignment, 0.f, 1.f);

        ImGui::End();

        for (auto& boid : boids)
        {
            boid.drawBoids3D(vertices, textureLadybug, boids, ProjMatrix, MVMatrix, uMVPMatrix, uMVMatrix, uNormalMatrix, uTextLadybug);
            boid.update_pos();
            boid.update_direction(boids);
        }

        // Debind VAO
        vao.unbind();

        glBindTexture(GL_TEXTURE_2D, 0);
    };

    ctx.start();

    glDeleteTextures(1, &textureLadybug);

    return 0;
}
