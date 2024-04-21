#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "../src/model/model.hpp"
#include "../src/person/person.hpp"
#include "../src/probability/proba.hpp"
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

// Fonctions et variables pour les mathematiques

// POISSON
glm::vec3 oceanPosition = glm::vec3(0., -5., -1.);
bool      stormHandled  = false;

void raise_ocean_level()
{
    if (!stormHandled) // Vérifie si la tempête n'a pas encore été gérée
    {
        // Augmenter la position de l'océan vers le haut une seule fois
        oceanPosition.y += 0.8f;
        stormHandled = true; // Marquer que la tempête a été gérée
    }
}

// CAUCHY
glm::vec3 barque_position = glm::vec3(2., -4., -8.);

// LOGISTIQUE
// double L  = 75.0;
// double q  = 0.1;
// double x0 = 0.5;
// int nbr_boids = nombre_boids_initial(L, q, x0);

int main()
{
    auto ctx = p6::Context{{window_width, window_height, "Boids"}};
    ctx.maximize_window();

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    // std::vector<Boids> boids(nbr_boids);
    std::vector<Boids> boids(75);
    for (auto& boid : boids)
    {
        boid.set_speed();
        boid.set_position();
    }

    //  Load shaders

    LoadShader ShaderLight("shaders/3D.vs.glsl", "shaders/lights.fs.glsl");

    LoadShader ShaderCube("shaders/3D.vs.glsl", "shaders/text2D.fs.glsl");

    LoadShader ShaderSphere("shaders/3D.vs.glsl", "shaders/normals.fs.glsl");

    // Load texture
    img::Image img_ladybug = p6::load_image_buffer("assets/textures/dragonfly.png");
    img::Image img_person  = p6::load_image_buffer("assets/textures/titan.png");

    // objects
    img::Image img_barque = p6::load_image_buffer("assets/textures/barque.jpg");
    img::Image img_ocean  = p6::load_image_buffer("assets/textures/ocean.png");
    img::Image img_ile    = p6::load_image_buffer("assets/textures/ile.jpg");
    img::Image img_trees  = p6::load_image_buffer("assets/textures/trees.jpg");
    img::Image img_pirate = p6::load_image_buffer("assets/textures/pirate.jpg");
    img::Image img_cube   = p6::load_image_buffer("assets/textures/aerial_rocks_02_diff_4k.jpg");

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

    // recup variable uniforme
    GLint uMVPMatrix    = glGetUniformLocation(ShaderSphere.getShaderID(), "uMVPMatrix");
    GLint uMVMatrix     = glGetUniformLocation(ShaderSphere.getShaderID(), "uMVMatrix");
    GLint uNormalMatrix = glGetUniformLocation(ShaderSphere.getShaderID(), "uNormalMatrix");

    Model ladybug = Model();
    ladybug.loadModel("dragonfly.obj");
    Model personModel = Model();
    personModel.loadModel("titan.obj");
    Model barque = Model();
    barque.loadModel("barque.obj");
    Model ocean = Model();
    ocean.loadModel("ocean.obj");
    Model ile = Model();
    ile.loadModel("ile.obj");
    Model trees = Model();
    trees.loadModel("trees.obj");
    Model pirate = Model();
    pirate.loadModel("pirate.obj");

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

    GLuint textureIle;
    glGenTextures(1, &textureIle);
    glBindTexture(GL_TEXTURE_2D, textureIle);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_ile.width(), img_ile.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img_ile.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint textureTrees;
    glGenTextures(1, &textureTrees);
    glBindTexture(GL_TEXTURE_2D, textureTrees);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_trees.width(), img_trees.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img_trees.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint pirateTexture;
    glGenTextures(1, &pirateTexture);
    glBindTexture(GL_TEXTURE_2D, pirateTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_pirate.width(), img_pirate.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img_pirate.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Sphere

    VBO vbo;
    vbo.bind();

    std::vector<glimac::ShapeVertex> vertices = glimac::sphere_vertices(0.5, 32, 16);
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

    // Initialisation de la texture
    ladybug.setVbo();
    ladybug.setVao();
    personModel.setVbo();
    personModel.setVao();
    barque.setVbo();
    barque.setVao();
    ocean.setVbo();
    ocean.setVao();
    ile.setVbo();
    ile.setVao();
    trees.setVbo();
    trees.setVao();
    pirate.setVbo();
    pirate.setVao();

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

    Person personCam;
    Camera camera(personCam, personModel);
    bool   right = false;
    bool   left  = false;
    bool   up    = false;
    bool   down  = false;
    // glm::vec3 personPosition = personCam.getPosition();

    // light

    Light lightScene = Light(glm::vec3{100});

    Light lightPerson = Light(glm::vec3{0.2});

    // VARIABLE AND TOOLS FOR MATH

    // POISSON

    srand(time(0)); // A verifier qi on a le droit d'utiliser ça

    double random_value = (double)rand() / RAND_MAX;
    double lambda       = 0.6; // lambda = 0.6 --> Tempête = OUI 45.12% du temps
    bool   stormHandled = false;
    bool   storm        = storm_occurs(lambda, random_value);
    std::cout << "Tempete : " << (storm ? "Oui" : "Non") << std::endl;

    // CAUCHY
    const double location_barque = 0.0; // Position centrale de la distribution de Cauchy
    const double scale_barque    = 1.0; // Échelle de la distribution de Cauchy

    if (cauchy_move_probability(location_barque, scale_barque))
    {
        // La barque se déplace
        std::cout << "La barque se deplace." << std::endl;
        barque_position.x += 1.0f;
    }
    else
    {
        // La barque ne se déplace pas
        std::cout << "La barque ne se deplace pas." << std::endl;
        barque_position = barque_position;
    }

    // LAPLACE
    double mu = 0.7;
    double b  = 0.1; // Paramètre d'échelle, peut être ajusté pour contrôler la dispersion des valeurs
    // double    mu          = 0.99;
    // double    b           = 0.05;
    glm::vec3 sphereColor = glm::vec3(1.0f, 1.0f, 0.0f); // Jaune
    bool      changeColor = laplace_color_probability(mu, b);

    // PARETO
    double    alpha       = 1.25; // plus alpha est grand plus la taille des îles se rapprochera de xmin
    double    xmin        = 0.1;
    double    xmax        = 0.5;
    double    island_size = generate_island_size(alpha, xmin, xmax);
    glm::vec3 island_scale(island_size); // island_size est converti en glm::vec3

    // GEOMETRIQUE

    const double p           = 0.25; // p est entre 0 et 1
    int          num_islands = geometric_distribution(p);
    std::cout << "Nombre d'iles' : " << num_islands << std::endl;

    // Déterminer les positions prédéfinies des îles
    std::vector<glm::vec3> island_positions = {
        glm::vec3(-4., -5.5, -4.),
        glm::vec3(2., -5.5, 4.),
        glm::vec3(-3., -5.5, 5.),
        glm::vec3(3., -5.5, -3.),
        glm::vec3(3.6, -5.5, -2.),
    };

    // UNIFORME
    //  Vérifier si le bateau pirate est présent avec une loi uniforme
    char pirate_present = random_letter_uniform();

    // GAMMA
    double    theta      = 1;
    double    k          = 2;
    double    taille_min = 0.05;
    double    taille_max = 0.23;
    double    tree_size  = taille_arbre_gamma(k, theta, taille_min, taille_max);
    glm::vec3 tree_scale(tree_size);

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

        lightScene.drawLightScene(glm::vec3(0., 4, 2.), ProjMatrix, viewMatrix, ShaderLight);
        lightPerson.drawLightPerson(personCam.getPosition(), ProjMatrix, viewMatrix, ShaderLight);

        personCam.drawPerson(personModel, viewMatrix, ProjMatrix, ShaderLight, texturePerson);

        ShaderSphere.use();

        if (changeColor)
        {
            sphereColor = glm::vec3(1.0f, 0.0f, 0.0f); // Rouge
        }

        // Passer la couleur au shader
        GLint uColorLocation = glGetUniformLocation(ShaderSphere.getShaderID(), "uColor");
        glUniform3fv(uColorLocation, 1, glm::value_ptr(sphereColor));

        // Liaison du VAO de la sphère
        vao.bind();

        // Appliquer les transformations
        MVMatrix     = glm::translate(glm::mat4(1.0), glm::vec3(2., 1., -5.));
        MVMatrix     = glm::rotate(MVMatrix, -ctx.time(), glm::vec3(0, 1, 0));
        NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        // Passez les matrices au shader
        glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
        glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

        // Dessinez la sphère
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        // Libération du VAO
        vao.unbind();

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

        barque.draw(barque_position, glm::vec3{1.}, ProjMatrix, viewMatrix, ShaderLight, textureBarque);

        // Update the scene according to storm
        if (storm)
        {
            raise_ocean_level();
        }
        ocean.draw(oceanPosition, glm::vec3{5.}, ProjMatrix, viewMatrix, ShaderLight, textureOcean);

        for (int j = 0; j < num_islands; ++j)
        {
            if (j < island_positions.size())
            {
                // Utiliser la position prédéfinie pour dessiner l'île
                ile.draw(island_positions[j], island_scale, ProjMatrix, viewMatrix, ShaderLight, textureIle);
            }
        }

        trees.draw(glm::vec3(-2.5, -4., -3.5), tree_scale, ProjMatrix, viewMatrix, ShaderLight, textureTrees);

        if (pirate_present == 'a')
        {
        }
        else if (pirate_present == 'p')
        {
            pirate.draw(glm::vec3(3.5, -5., -5.0), glm::vec3{0.3}, ProjMatrix, viewMatrix, ShaderLight, pirateTexture);
        }

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
    ile.~Model();
    trees.~Model();
    pirate.~Model();

    cube.~Cube();

    lightScene.~Light();
    lightPerson.~Light();

    glDeleteTextures(1, &textureLadybug);
    glDeleteTextures(1, &texturePerson);
    glDeleteTextures(1, &textureBarque);
    glDeleteTextures(1, &textureOcean);
    glDeleteTextures(1, &textureIle);
    glDeleteTextures(1, &textureTrees);
    glDeleteTextures(1, &pirateTexture);

    return 0;
}