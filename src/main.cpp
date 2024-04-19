#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "../src/model/model.hpp"
#include "../src/person/person.hpp"
#include "../src/scene/camera.hpp"
#include "../src/scene/cube.hpp"
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

const int window_width  = 1280;
const int window_height = 720;

int main()
{
    auto ctx = p6::Context{{window_width, window_height, "Boids"}};
    ctx.maximize_window();

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    std::vector<Boids> boids(75);
    for (auto& boid : boids)
    {
        boid.set_speed();
        boid.set_position();
    }
    // Load shaders

    LoadShader ShaderLight("shaders/3D.vs.glsl", "shaders/lights.fs.glsl");

    LoadShader ShaderCube("shaders/3D.vs.glsl", "shaders/text2D.fs.glsl");

    // Load texture
    img::Image img_ladybug = p6::load_image_buffer("assets/textures/dragonfly.png");
    img::Image img_person  = p6::load_image_buffer("assets/textures/titan.png");
    img::Image img_barque  = p6::load_image_buffer("assets/textures/barque.jpg");
    img::Image img_ocean   = p6::load_image_buffer("assets/textures/ocean.png");

    img::Image img_cube = p6::load_image_buffer("assets/textures/aerial_rocks_02_diff_4k.jpg");

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

    ShaderCube.addUniformVariable("uTexture");
    ShaderCube.addUniformVariable("uMVPMatrix");
    ShaderCube.addUniformVariable("uMVMatrix");
    ShaderCube.addUniformVariable("uNormalMatrix");

    Model ladybug = Model();
    ladybug.loadModel("dragonfly.obj");
    Model personModel = Model();
    personModel.loadModel("titan.obj");
    Model barque = Model();
    barque.loadModel("barque.obj");
    Model ocean = Model();
    ocean.loadModel("ocean.obj");

    // Initialisation de la texture
    GLuint textureLadybug;
    glGenTextures(1, &textureLadybug);
    glBindTexture(GL_TEXTURE_2D, textureLadybug);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_ladybug.width(), img_ladybug.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img_ladybug.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint texturePerson;
    glGenTextures(1, &texturePerson);
    glBindTexture(GL_TEXTURE_2D, texturePerson);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_person.width(), img_person.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img_person.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint textureBarque;
    glGenTextures(1, &textureBarque);
    glBindTexture(GL_TEXTURE_2D, textureBarque);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_barque.width(), img_barque.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img_barque.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint textureOcean;
    glGenTextures(1, &textureOcean);
    glBindTexture(GL_TEXTURE_2D, textureOcean);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_ocean.width(), img_ocean.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img_ocean.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Initialisation de la texture
    ladybug.setVbo();
    ladybug.setVao();
    personModel.setVbo();
    personModel.setVao();
    barque.setVbo();
    barque.setVao();
    ocean.setVbo();
    ocean.setVao();

    Cube cube(5.f);
    cube.init(img_cube);

    // Option to see tests in depth?
    glEnable(GL_DEPTH_TEST);

    // Calcul des différentes matrices nécessaires pour les shaders
    glm::mat4 ProjMatrix;
    glm::mat4 MVMatrix;
    glm::mat4 NormalMatrix;

    ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);

    // camera

    Person    personCam;
    Camera    camera(personCam, personModel);
    bool      right          = false;
    bool      left           = false;
    bool      up             = false;
    bool      down           = false;
    glm::vec3 personPosition = personCam.getPosition();

    // light

    Light lightScene = Light(glm::vec3{100});

    Light lightPerson = Light(glm::vec3{0.1});

    /* Loop until the user closes the window */
    ctx.update = [&]() {
        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        // camera
        cameraOption(personCam, left, right, up, down, ctx);

        glm::mat4 viewMatrix = personCam.getViewMatrix();
        camera.getViewMatrix(viewMatrix);

        ShaderLight.use();

        glClearColor(0.2f, 0.2f, 0.2f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        MVMatrix     = glm::translate(glm::mat4(1.0), glm::vec3(0., -10., -5.));
        MVMatrix     = viewMatrix * MVMatrix;
        NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        lightScene.drawLightScene(glm::vec3(0., 0, 0.), ProjMatrix, viewMatrix, ShaderLight);
        lightPerson.drawLightPerson(personCam.getPosition(), ProjMatrix, viewMatrix, ShaderLight);

        personCam.drawPerson(personModel, viewMatrix, ProjMatrix, ShaderLight, texturePerson);

        ShaderCube.use();
        cube.draw(glm::vec3(0., -5., -5.), glm::vec3{5.}, ShaderCube, viewMatrix, ProjMatrix);
        cube.borders(personCam);

        // Gui
        ImGui::Begin("Slider");
        ImGui::SliderFloat("CohesionForce", &Boids::cohesion_force, 0.f, 1.f);
        ImGui::SliderFloat("SeparationForce", &Boids::separation_force, 0.f, 1.f);
        ImGui::SliderFloat("Distance to unite", &Boids::distance_cohesion, 0.f, 1.f);
        ImGui::SliderFloat("Distance to escape", &Boids::distance_separation, 0.f, 1.f);
        ImGui::SliderFloat("Distance to align", &Boids::distance_alignment, 0.f, 1.f);

        ImGui::End();

        // Draw boids
        ShaderLight.use();

        barque.draw(glm::vec3(0., -2., -5.), glm::vec3{1.}, ProjMatrix, viewMatrix, ShaderLight, textureBarque);

        ocean.draw(glm::vec3(0., -5., 0.), glm::vec3{5.}, ProjMatrix, viewMatrix, ShaderLight, textureOcean);

        for (auto& boid : boids)
        {
            boid.drawBoids3D(ladybug, ProjMatrix, NormalMatrix, viewMatrix, ShaderLight, textureLadybug);
            boid.update_pos();
            boid.update_direction(boids);
        }
    };

    ctx.start();

    ladybug.~Model();
    personModel.~Model();
    barque.~Model();
    ocean.~Model();

    cube.~Cube();

    lightScene.~Light();
    lightPerson.~Light();

    glDeleteTextures(1, &textureLadybug);
    // glDeleteTextures(1, &texturePerson);
    glDeleteTextures(1, &textureBarque);
    glDeleteTextures(1, &textureOcean);

    return 0;
}