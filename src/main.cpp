#include "p6/p6.h"

struct Vertex2DColor {
    glm::vec2 position;
    glm::vec3 color;
};

// class avec destructeur = un element seulement
class VBO {
public:
    VBO()
    {
        glGenBuffers(1, &m_id);
    }

    ~VBO()
    {
        glDeleteBuffers(1, &m_id);
    }
    VBO(const VBO&)            = delete;
    VBO& operator=(const VBO&) = delete;

    VBO(VBO&& other) noexcept // Move constructor
        : m_id{other.m_id}
    {
        other.m_id = 0; // Make sure that other won't delete the m_id we just copied
    }
    VBO& operator=(VBO&& other) noexcept // Move assignment operator
    {
        if (this != &other)
        {                              // Make sure that we don't do silly things when we try to move an object to itself
            glDeleteBuffers(1, &m_id); // Delete the previous object
            m_id       = other.m_id;   // Copy the object
            other.m_id = 0;            // Make sure that other won't delete the m_id we just copied
        }
        return *this; // move assignment must return a reference to this, so we do it
    }

    GLuint id() const { return m_id; } // The getter for the wrapped `_id`.

    void bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
    }

    void unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void setData(const void* data, size_t size)
    {
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

private:
    GLuint m_id{};
};

class VAO {
public:
    VAO()
    {
        glGenVertexArrays(1, &m_id);
    }

    ~VAO()
    {
        glDeleteVertexArrays(1, &m_id);
    }
    VAO(const VAO&)            = delete;
    VAO& operator=(const VAO&) = delete;

    VAO(VAO&& other) noexcept // Move constructor
        : m_id{other.m_id}
    {
        other.m_id = 0; // Make sure that other won't delete the m_id we just copied
    }
    VAO& operator=(VAO&& other) noexcept // Move assignment operator
    {
        if (this != &other)
        {                                   // Make sure that we don't do silly things when we try to move an object to itself
            glDeleteVertexArrays(1, &m_id); // Delete the previous object
            m_id       = other.m_id;        // Copy the object
            other.m_id = 0;                 // Make sure that other won't delete the m_id we just copied
        }
        return *this; // move assignment must return a reference to this, so we do it
    }

    GLuint id() const { return m_id; } // The getter for the wrapped `_id`.

    void bind() const
    {
        glBindVertexArray(m_id);
    }

    void unbind()
    {
        glBindVertexArray(0);
    }

private:
    GLuint m_id{};
};

int main()
{
    auto ctx = p6::Context{{1280, 720, "Boids"}};
    ctx.maximize_window();

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/
    const p6::Shader shader = p6::load_shader(
        "shaders/triangle.vs.glsl",
        "shaders/triangle.fs.glsl"
    );

    // Declare your infinite update loop.
    ctx.update = [&]() {
        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/
        VBO vbo;

        vbo.bind();

        Vertex2DColor vertices[] = {
            Vertex2DColor{{-0.5f, -0.5f}, {1.f, 0.f, 0.f}}, // Premier sommet
            Vertex2DColor{{-0.5f, 0.5f}, {0.f, 1.f, 0.f}},  // Deuxième sommet
            Vertex2DColor{{0.5f, -0.5f}, {0.f, 0.f, 1.f}},  // Troisième sommet
            Vertex2DColor{{0.5f, 0.5f}, {1.f, 0.f, 0.f}},   // Premier sommet
            Vertex2DColor{{-0.5f, 0.5f}, {0.f, 1.f, 0.f}},  // Deuxième sommet
            Vertex2DColor{{0.5f, -0.5f}, {0.f, 0.f, 1.f}}   // Troisième sommet
        };

        vbo.setData(vertices, sizeof(vertices));
        vbo.unbind();

        VAO vao;
        vao.bind();

        const GLuint VERTEX_ATTR_POSITION = 3;
        const GLuint VERTEX_ATTR_COLOR    = 8;
        glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
        glEnableVertexAttribArray(VERTEX_ATTR_COLOR);
        vbo.bind();
        glVertexAttribPointer(VERTEX_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), (const GLvoid*)offsetof(Vertex2DColor, position));

        glVertexAttribPointer(VERTEX_ATTR_COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), (const GLvoid*)offsetof(Vertex2DColor, color));
        vbo.unbind();
        vao.unbind();

        glClear(GL_COLOR_BUFFER_BIT);
        vao.bind();

        shader.use();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        vao.unbind();
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}
