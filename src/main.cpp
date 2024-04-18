#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "../src/model/model.hpp"
#include "../src/scene/camera.hpp"
#include "../src/scene/lights.hpp"
#include "../src/scene/loadShader.hpp"
#include "../src/tools/vbovao.hpp"
#include "GLFW/glfw3.h"
#include "boids/boids.hpp"
#include "glimac/FreeflyCamera.hpp"
#include "glimac/TrackballCamera.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/random.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "img/src/Image.h"
#include "p6/p6.h"

int main()
{
    auto ctx = p6::Context{{window_width, window_height, "Boids"}};
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

    LoadShader ShaderLight("shaders/3D.vs.glsl", "shaders/lights.fs.glsl");

    // Load texture
    img::Image img_ladybug = p6::load_image_buffer("assets/models/Ladybug.jpg");

    // Retrieve uniform variables Light
    ShaderLight.addUniformVariable("uMVPMatrix");
    ShaderLight.addUniformVariable("uMVMatrix");
    ShaderLight.addUniformVariable("uNormalMatrix");
    ShaderLight.addUniformVariable("uLightPos_vs");
    ShaderLight.addUniformVariable("uLightIntensity");
    ShaderLight.addUniformVariable("uLightPos2_vs");
    ShaderLight.addUniformVariable("uLightIntensity2");
    ShaderLight.addUniformVariable("uKd");
    ShaderLight.addUniformVariable("uKs");
    ShaderLight.addUniformVariable("uShininess");
    ShaderLight.addUniformVariable("uTextLadybug");

    // Initialisation de la texture
    GLuint textureLadybug;
    glGenTextures(1, &textureLadybug);
    glBindTexture(GL_TEXTURE_2D, textureLadybug);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_ladybug.width(), img_ladybug.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img_ladybug.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Creation du VBO
    VBO vboBoids;
    vboBoids.bind();

    // Creation de la forme de la sphère
    const std::vector<glimac::ShapeVertex> vertices = glimac::sphere_vertices(1.f, 32, 16);

    // Fill coordinates in the VBO (Static is for constant variables)
    vboBoids.setData(vertices.data(), vertices.size() * sizeof(glimac::ShapeVertex));

    // Option to see tests in depth?
    glEnable(GL_DEPTH_TEST);

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
    vboBoids.unbind();

    // Debind du VAO
    vao.unbind();

    // Calcul des différentes matrices nécessaires pour les shaders
    glm::mat4 ProjMatrix;
    glm::mat4 MVMatrix;
    glm::mat4 NormalMatrix;

    ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);

    // camera
    Camera camera;

    // light

    Light lightScene = Light(glm::vec3{1000});
    // Light light2 = Light(glm::vec3{0});

    /* Loop until the user closes the window */
    ctx.update = [&]() {
        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        // camera

        ctx.key_pressed = [&camera](const p6::Key& key) {
            camera.handleKeyPressed(key);
        };
        ctx.key_released = [&camera](const p6::Key& key) {
            camera.handleKeyReleased(key);
        };
        ctx.mouse_dragged = [&camera](const p6::MouseDrag& button) {
            camera.handleMouseDragged(button);
        };

        ctx.mouse_scrolled = [&camera](const p6::MouseScroll& scroll) {
            camera.handleMouseScrolled(scroll);
        };

        glm::mat4 viewMatrix = camera.getViewMatrix();

        ShaderLight.use();

        glClearColor(0.2f, 0.2f, 0.2f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Bind VAO
        vao.bind();

        // Bind texture Ladybug
        glBindTexture(GL_TEXTURE_2D, textureLadybug);

        // Apply matrices in the shaders
        ShaderLight.setUniform4fv("uMVPMatrix", ProjMatrix * MVMatrix);
        ShaderLight.setUniform4fv("uMVMatrix", MVMatrix);
        ShaderLight.setUniform4fv("uNormalMatrix", NormalMatrix);

        // lights
        lightScene.drawLightScene(glm::vec3{0.f, 0.f, -1.0f}, ProjMatrix, viewMatrix, ShaderLight);
        // light2.drawLightPlayer(glm::vec3{0.f, 0.f, 0.f}, ProjMatrix, viewMatrix, ShaderLight);

        // Gui
        ImGui::Begin("Slider");
        ImGui::SliderFloat("CohesionForce", &Boids::cohesion_force, 0.f, 1.f);
        ImGui::SliderFloat("SeparationForce", &Boids::separation_force, 0.f, 1.f);
        ImGui::SliderFloat("Distance to unite", &Boids::distance_cohesion, 0.f, 1.f);
        ImGui::SliderFloat("Distance to escape", &Boids::distance_separation, 0.f, 1.f);
        ImGui::SliderFloat("Distance to align", &Boids::distance_alignment, 0.f, 1.f);

        ImGui::End();

        // Draw boids

        for (auto& boid : boids)
        {
            boid.drawBoids3D(vertices, boids, ProjMatrix, NormalMatrix, viewMatrix, ShaderLight, textureLadybug);
            boid.update_pos();
            boid.update_direction(boids);
        }

        // Debind VAO
        vao.unbind();

        glBindTexture(GL_TEXTURE_2D, 0);
    };

    ctx.start();

    lightScene.~Light();
    // light2.~Light();

    glDeleteTextures(1, &textureLadybug);

    return 0;
}