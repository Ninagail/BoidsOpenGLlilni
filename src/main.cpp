#include <iostream>
#include <vector>
#include "../src/scene/lights.hpp"
#include "../src/scene/loadShader.hpp"
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
    LoadShader ShaderText("shaders/3D.vs.glsl", "shaders/text3D.fs.glsl");

    LoadShader ShaderLight("shaders/3D.vs.glsl", "shaders/lights.fs.glsl");

    // Load texture
    img::Image img_ladybug = p6::load_image_buffer("assets/models/Ladybug.jpg");

    // Retrieve uniform variables Text
    ShaderText.addUniformVariable("uMVPMatrix");
    ShaderText.addUniformVariable("uMVMatrix");
    ShaderText.addUniformVariable("uNormalMatrix");
    ShaderText.addUniformVariable("uTextLadybug");

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
    ShaderLight.addUniformVariable("uText");

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
    vboBoids.unbind();

    // Debind du VAO
    vao.unbind();

    // Calcul des différentes matrices nécessaires pour les shaders
    glm::mat4 ProjMatrix;
    glm::mat4 MVMatrix;
    glm::mat4 NormalMatrix;

    ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);

    // light

    Light light1 = Light(glm::vec3{100000.});
    Light light2 = Light(glm::vec3{0.001});

    /* Loop until the user closes the window */
    ctx.update = [&]() {
        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        ShaderLight.use();
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

        // lights
        light1.drawLightScene(glm::vec3(0.0, 5, -5.0), ProjMatrix, MVMatrix, ShaderLight);
        light2.drawLightPlayer(glm::vec3(0.0, 0.0, 0.0), ProjMatrix, MVMatrix, ShaderLight);

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
            boid.drawBoids3D(vertices, boids, ProjMatrix, NormalMatrix, MVMatrix, ShaderText, textureLadybug);
            boid.update_pos();
            boid.update_direction(boids);
        }

        // Debind VAO
        vao.unbind();

        glBindTexture(GL_TEXTURE_2D, 0);
    };

    ctx.start();

    light1.~Light();
    light2.~Light();

    glDeleteTextures(1, &textureLadybug);

    return 0;
}
