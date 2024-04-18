#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "../src/model/model.hpp"
#include "../src/scene/camera.hpp"
// #include "../src/scene/lights.hpp"
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

struct Light {
    p6::Shader m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    // texture comment
    //  GLint uTexture;

    GLint uKd;
    GLint uKs;
    GLint uShininess;
    GLint uLightDir_vs;
    GLint uLightIntensity;

    Light()
        : m_Program{p6::load_shader("shaders/3D.vs.glsl", "shaders/lights.fs.glsl")}
    {
        uMVPMatrix    = glGetUniformLocation(m_Program.id(), "uMVPMatrix");
        uMVMatrix     = glGetUniformLocation(m_Program.id(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.id(), "uNormalMatrix");
        // texture comment
        //  uTexture      = glGetUniformLocation(m_Program.id(), "uTexture");
        uKd             = glGetUniformLocation(m_Program.id(), "uKd");
        uKs             = glGetUniformLocation(m_Program.id(), "uKs");
        uShininess      = glGetUniformLocation(m_Program.id(), "uShininess");
        uLightDir_vs    = glGetUniformLocation(m_Program.id(), "uLightDir_vs");
        uLightIntensity = glGetUniformLocation(m_Program.id(), "uLightIntensity");
    }
};

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
    LoadShader ShaderText("shaders/3D.vs.glsl", "shaders/lights.fs.glsl");

    // LoadShader ShaderLight("shaders/3D.vs.glsl", "shaders/lights.fs.glsl");
    Light light{};

    // Load texture
    img::Image img_ladybug = p6::load_image_buffer("assets/models/Ladybug.jpg");

    // Retrieve uniform variables Text
    ShaderText.addUniformVariable("uMVPMatrix");
    ShaderText.addUniformVariable("uMVMatrix");
    ShaderText.addUniformVariable("uNormalMatrix");
    ShaderText.addUniformVariable("uTextLadybug");

    // Retrieve uniform variables Light
    // ShaderLight.addUniformVariable("uMVPMatrix");
    // ShaderLight.addUniformVariable("uMVMatrix");
    // ShaderLight.addUniformVariable("uNormalMatrix");
    // ShaderLight.addUniformVariable("uLightPos_vs");
    // ShaderLight.addUniformVariable("uLightIntensity");
    // ShaderLight.addUniformVariable("uLightPos2_vs");
    // ShaderLight.addUniformVariable("uLightIntensity2");
    // ShaderLight.addUniformVariable("uKd");
    // ShaderLight.addUniformVariable("uKs");
    // ShaderLight.addUniformVariable("uShininess");
    // ShaderLight.addUniformVariable("uText");

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

    // std::vector<glimac::ShapeVertex> objVertices;
    // if (!loadOBJ("assets/models/chiamaia.obj", objVertices))
    //  {
    //      std::cerr << "Failed to load 3D object" << std::endl;
    //      return -1;
    //  }

    // Creation de la forme de la sphère
    const std::vector<glimac::ShapeVertex> vertices = glimac::sphere_vertices(1.f, 32, 16);

    // Fill coordinates in the VBO (Static is for constant variables)
    // glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glimac::ShapeVertex), vertices.data(), GL_STATIC_DRAW);
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

    // Light light1 = Light(glm::vec3{100000.}, glm::vec3{0., 0., 0.});
    // Light light2 = Light(glm::vec3{0.001}, glm::vec3{0., 0., 0.});

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

        // ShaderLight.use();
        light.m_Program.use();
        glClearColor(0.2f, 0.2f, 0.2f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Bind VAO
        vao.bind();

        // Bind texture Ladybug
        glBindTexture(GL_TEXTURE_2D, textureLadybug);

        const glm::mat4 globalMVMatrix = glm::translate(glm::mat4{1.f}, {0.f, 0.f, -5.f});
        const glm::mat4 lightMMatrix   = glm::rotate(globalMVMatrix, ctx.time(), {0.f, 1.f, 0.f});
        const glm::mat4 lightMVMatrix  = viewMatrix * lightMMatrix; // variation de la MV (simuler une cam)
        const glm::mat4 lightMVPMatrix = ProjMatrix * lightMVMatrix;
        // lights
        glUniform3fv(light.uKd, 1, glm::value_ptr(glm::vec3(0.5, 0.3, 0.3))); // coefficient glossy
        glUniform3fv(light.uKs, 1, glm::value_ptr(glm::vec3(0.8, 0.2, 0.3))); // coef de reflexion
        glUniform1f(light.uShininess, 0.6f);                                  // brillance
        glm::vec3 lightdir(1., 1., 1.);
        lightdir                     = (lightMVMatrix * glm::vec4(lightdir, 0.));
        const glm::mat4 normalMatrix = glm::transpose(glm::inverse(lightMVMatrix));

        glUniform3fv(light.uLightDir_vs, 1, glm::value_ptr(lightdir));
        glUniform3fv(light.uLightIntensity, 1, glm::value_ptr(glm::vec3(2.0, 2.0, 2.0)));

        glUniformMatrix4fv(light.uMVMatrix, 1, GL_FALSE, glm::value_ptr(lightMVMatrix));
        glUniformMatrix4fv(light.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));
        glUniformMatrix4fv(light.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(lightMVPMatrix));

        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        // Ladybug
        // MVMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0., 0., -5.));
        // MVMatrix     = glm::rotate(MVMatrix, glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
        // NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        ShaderText.use();
        // Set texture Ladybug
        ShaderText.setUniform1i("uTextLadybug", 0);

        // Apply matrices in the shaders
        ShaderText.setUniform4fv("uMVPMatrix", ProjMatrix * MVMatrix);
        ShaderText.setUniform4fv("uMVMatrix", MVMatrix);
        ShaderText.setUniform4fv("uNormalMatrix", NormalMatrix);

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
            boid.drawBoids3D(vertices, boids, ProjMatrix, NormalMatrix, viewMatrix, ShaderText, textureLadybug);
            boid.update_pos();
            boid.update_direction(boids);
        }

        // Debind VAO
        vao.unbind();

        glBindTexture(GL_TEXTURE_2D, 0);
    };

    ctx.start();

    // light1.~Light();
    // light2.~Light();

    glDeleteTextures(1, &textureLadybug);

    return 0;
}
