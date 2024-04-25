#include "glimac/common.hpp"
#include "glm/glm.hpp"
#include "p6/p6.h"

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