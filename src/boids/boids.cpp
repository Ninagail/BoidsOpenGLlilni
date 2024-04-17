#include "../src/boids/boids.hpp"
#include <cstdlib>
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

static constexpr glm::vec3 speedMax = glm::vec3(0.02f, 0.02f, 0.02f);
// initialisation stattic
float Boids::distance_alignment  = 0.1f;
float Boids::distance_separation = 0.05f;
float Boids::distance_cohesion   = 0.2f;
float Boids::separation_force    = 0.01f;
float Boids::cohesion_force      = 0.2f;

void Boids::drawBoids(p6::Context& ctx) const
{
    ctx.fill = {0.3f, 0.5f, 0.9f, 1.0f};
    ctx.circle(p6::Center{this->get_position().x, this->get_position().y}, p6::Radius{0.025f});
    ctx.use_stroke = false;
}

void Boids::drawBoids3D(const std::vector<glimac::ShapeVertex>& vertices, const std::vector<Boids>& boids, glm::mat4 ProjMatrix, glm::mat4 NormalMatrix, glm::mat4 viewMatrix, LoadShader& shader, GLuint textName)

{
    // Bind texture Ladybug
    // glBindTexture(GL_TEXTURE_2D, textureLadybug);

    // // Set texture Ladybug
    // glUniform1i(uTextLadybug, 0);

    // glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glimac::ShapeVertex), vertices.data(), GL_STATIC_DRAW);
    for (const auto& boid : boids)
    {
        // Calculate the model matrix for the boid
        glm::mat4 MVMatrixBoids = glm::translate(glm::mat4{1.f}, {0.f, 0.f, -3.f});                        // Translation
        MVMatrixBoids           = glm::rotate(MVMatrixBoids, glm::radians(180.0f), glm::vec3(0., 1., 0.)); // Translation * Rotation
        MVMatrixBoids           = glm::translate(MVMatrixBoids, this->m_position);                         // Translation * Rotation * Translation

        // glm::mat4 MVMatrixBoids = glm::translate(glm::mat4{1.f}, this->m_pos); // Translation

        MVMatrixBoids = glm::scale(MVMatrixBoids, glm::vec3{this->m_size}); // Translation * Rotation * Translation * Scale
        MVMatrixBoids = viewMatrix * MVMatrixBoids;

        shader.setUniform4fv("uMVPMatrix", ProjMatrix * MVMatrixBoids);
        shader.setUniform4fv("uMVMatrix", MVMatrixBoids);
        shader.setUniform4fv("uNormalMatrix", NormalMatrix);

        // Fill coordinates in the VBO (Static is for constant variables)
        // glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glimac::ShapeVertex), vertices.data(), GL_STATIC_DRAW);

        // Draw the boid
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    }

    // Unbind texture and VAO
    // glBindTexture(GL_TEXTURE_2D, 0);
}

void Boids::update_pos()
{
    m_position += m_speed;

    if (m_position.x <= -1.975f)
    {
        m_position.x += 2.95f;
    }
    else if (m_position.x >= 1.975f)
    {
        m_position.x -= 2.95f;
    }

    if (m_position.y <= -1.975f)
    {
        m_position.y += 2.95f;
    }
    else if (m_position.y >= 1.975f)
    {
        m_position.y -= 2.95f;
    }
    if (m_position.z <= -1.975f)
    {
        m_position.z += 2.95f;
    }
    else if (m_position.z >= 1.975f)
    {
        m_position.z -= 2.95f;
    }
}

void Boids::update_direction(std::vector<Boids>& boids)
{
    for (auto& elem : boids)
    {
        elem.cohesion(boids);
        elem.alignment(boids);
        elem.separation(boids);
    }
    m_position += m_speed;
    this->update_pos();
}

/*LOI DES BOIDS*/

// Cohésion
void Boids::cohesion(const std::vector<Boids>& boids)
{
    glm::vec3 newPosition{0.0f, 0.0f, 0.0f};
    int       count = 0;

    for (auto& elem : boids)
    {
        if (&elem == this)
            continue;
        float distance = glm::length(elem.m_position - this->m_position);
        if (distance < distance_cohesion)
        {
            newPosition += (elem.m_position - m_position) * cohesion_force;
            count++;
        }
    }

    if (count > 0)
    {
        newPosition /= count;

        if (length(newPosition) > separation_force)
        {
            newPosition = glm::normalize(newPosition) * separation_force;
        }

        m_speed += (newPosition)*separation_force;
    }
}

// Alignement
void Boids::alignment(const std::vector<Boids>& boids)
{
    glm::vec3 newVelocity{0.0f, 0.0f, 0.0f};
    int       count = 0;

    for (auto& elem : boids)
    {
        const float distance = glm::length(elem.m_position - this->m_position);
        if (distance < distance_alignment)
        {
            newVelocity += elem.m_speed;
            count++;
        }
    }

    if (count > 0)
    {
        newVelocity /= count;
        if (length(newVelocity) > separation_force)
        {
            newVelocity = glm::normalize(newVelocity);
            m_speed     = newVelocity * speedMax;
        }
    }
}

// Séparation
void Boids::separation(const std::vector<Boids>& boids)

{
    glm::vec3 newDisplacement{0.0f, 0.0f, 0.0f};
    int       count = 0;
    for (auto& elem : boids)
    {
        if (&elem == this)
            continue;

        float distance = glm::length(elem.m_position - this->m_position);

        if (distance < distance_separation)
        {
            glm::vec3 difference = glm::normalize(this->m_position - elem.m_position);
            difference /= distance;
            newDisplacement += difference;
            count++;
        }
    }
    if (count > 0)
    {
        newDisplacement /= count;
        if (length(newDisplacement) > separation_force)
        {
            newDisplacement = glm::normalize(newDisplacement) * separation_force;
        }
        m_speed = newDisplacement;
    }
}

// SETTER

void Boids::set_position()

{
    m_position.x = p6::random::number(-0.975f, 0.975f);
    m_position.y = p6::random::number(-0.975f, 0.975f);
}

void Boids::set_speed()
{
    m_speed.x = p6::random::number(-0.01f, 0.01f);
    m_speed.y = p6::random::number(-0.01f, 0.01f);
}