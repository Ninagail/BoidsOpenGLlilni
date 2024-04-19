#include "../src/scene/cube.hpp"
#include "../src/scene/loadShader.hpp"

Cube::Cube(float size)
    : m_size(size), texture(0)
{
}

void Cube::textures(img::Image& img_load)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_load.width(), img_load.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img_load.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void Cube::vectors()
{
    // Calculer les coordonnées des sommets
    float x = m_size / 4.7f;
    float y = m_size / 3.0f;
    float z = m_size / 4.0f;

    vertices = {
        // Face avant
        -x, -y + 1, -z, // 0
        x, -y + 1, -z,  // 1
        x, y + 1, -z,   // 2
        -x, y + 1, -z,  // 3

        // Face arrière
        x, -y + 1, z,  // 4
        -x, -y + 1, z, // 5
        -x, y + 1, z,  // 6
        x, y + 1, z,   // 7

        // Face gauche
        -x, -y + 1, z,  // 8
        -x, -y + 1, -z, // 9
        -x, y + 1, -z,  // 10
        -x, y + 1, z,   // 11

        // Face droite
        x, -y + 1, -z, // 12
        x, -y + 1, z,  // 13
        x, y + 1, z,   // 14
        x, y + 1, -z,  // 15

        // Face supérieure
        -x, y + 1, -z, // 16
        x, y + 1, -z,  // 17
        x, y + 1, z,   // 18
        -x, y + 1, z,  // 19

        // Face inférieure
        -x, -y + 1, -z, // 20
        x, -y + 1, -z,  // 21
        x, -y + 1, z,   // 22
        -x, -y + 1, z   // 23
    };

    std::vector<float> textureCoords = {
        // Face avant
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,

        // Face arrière
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        // Face gauche
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        // Face droite
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,

        // Face supérieure
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,

        // Face inférieure
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f
    };

    glBufferData(GL_ARRAY_BUFFER, (vertices.size() + textureCoords.size()) * sizeof(float), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
    glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), textureCoords.size() * sizeof(float), textureCoords.data());

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    // Définir les indices des triangles
    indices = {
        // Face avant
        0, 1, 2,
        2, 3, 0,

        // Face arrière
        4, 5, 6,
        6, 7, 4,

        // Face gauche
        8, 9, 10,
        10, 11, 8,

        // Face droite
        12, 13, 14,
        14, 15, 12,

        // Face supérieure
        16, 17, 18,
        18, 19, 16,

        // Face inférieure
        20, 21, 22,
        22, 23, 20
    };

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
}

void Cube::vertex()
{
    const GLuint VERTEX_ATTR_POSITION = 0;
    const GLuint VERTEX_ATTR_NORMAL   = 1;
    const GLuint VERTEX_ATTR_UV       = 2;

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_UV);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glVertexAttribPointer(VERTEX_ATTR_UV, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(vertices.size() * sizeof(float)));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(2 * vertices.size() * sizeof(float)));

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void Cube::init(img::Image& img_load)
{
    textures(img_load);

    vectors();

    vertex();
}

void Cube::draw(glm::vec3 pos, glm::vec3 scale, LoadShader& loadShader, glm::mat4 viewMatrix, glm::mat4 ProjMatrix) const
{
    glm::mat4 ViewMatrixModel = glm::translate(glm::mat4(1.0), pos);
    ViewMatrixModel           = glm::scale(ViewMatrixModel, scale);

    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(ViewMatrixModel));

    ViewMatrixModel = viewMatrix * ViewMatrixModel;

    loadShader.setUniform4fv("uMVPMatrix", ProjMatrix * ViewMatrixModel);
    loadShader.setUniform4fv("uMVMatrix", ViewMatrixModel);
    loadShader.setUniform4fv("uNormalMatrix", NormalMatrix);

    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Cube::borders(Person& person)
{
    glm::vec3 personPosition = person.getPosition();

    float minX = -m_size / 1.5f;
    float maxX = m_size / 1.5f;
    float minZ = -m_size * 2;
    float maxZ = m_size / 6.5f;
    float minY = -m_size / 5.f;
    float maxY = m_size / 5.f;

    // Vérifier la coordonnée X
    if (personPosition.x < minX)
        person.setPosition(glm::vec3(minX, personPosition.y, personPosition.z));
    else if (personPosition.x > maxX)
        person.setPosition(glm::vec3(maxX, personPosition.y, personPosition.z));

    // Vérifier la coordonnée X
    if (personPosition.y < minY)
        person.setPosition(glm::vec3(personPosition.x, minY, personPosition.z));
    else if (personPosition.y > maxY)
        person.setPosition(glm::vec3(personPosition.x, maxY, personPosition.z));

    // Vérifier la coordonnée Z
    if (personPosition.z < minZ)
        person.setPosition(glm::vec3(personPosition.x, personPosition.y, minZ));
    else if (personPosition.z > maxZ)
        person.setPosition(glm::vec3(personPosition.x, personPosition.y, maxZ));
}