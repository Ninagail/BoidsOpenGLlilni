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

void Boids::drawBoids3D(Model& model, glm::mat4 ProjMatrix, glm::mat4 NormalMatrix, glm::mat4 viewMatrix, LoadShader& loadShader, GLuint textName)
{
    glm::mat4 MVMatrixBoids = glm::translate(glm::mat4{1.f}, {0.f, 0.f, -3.f});                        // Translation
    MVMatrixBoids           = glm::rotate(MVMatrixBoids, glm::radians(180.0f), glm::vec3(0., 1., 0.)); // Translation * Rotation
    MVMatrixBoids           = glm::translate(MVMatrixBoids, this->m_position);                         // Translation * Rotation * Translation

    // glm::mat4 MVMatrixBoids = glm::translate(glm::mat4{1.f}, this->m_pos); // Translation

    MVMatrixBoids = glm::scale(MVMatrixBoids, glm::vec3{this->m_size * 0.5f}); // Translation * Rotation * Translation * Scale
    MVMatrixBoids = viewMatrix * MVMatrixBoids;

    loadShader.setUniform1i("uTextLadybug", 0);
    loadShader.setUniform4fv("uMVPMatrix", ProjMatrix * MVMatrixBoids);
    loadShader.setUniform4fv("uMVMatrix", MVMatrixBoids);
    loadShader.setUniform4fv("uNormalMatrix", NormalMatrix);

    // glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrixBoids));
    // glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrixBoids));
    // glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

    model.drawArray(textName);
}

void Boids::update_pos()
{
    m_position += m_speed;

    if (m_position.x <= -3.975f)
    {
        m_position.x += 4.95f;
    }
    else if (m_position.x >= 3.975f)
    {
        m_position.x -= 4.95f;
    }

    if (m_position.y <= -3.975f)
    {
        m_position.y += 4.95f;
    }
    else if (m_position.y >= 3.975f)
    {
        m_position.y -= 4.95f;
    }
    if (m_position.z <= -3.975f)
    {
        m_position.z += 4.95f;
    }
    else if (m_position.z >= 3.975f)
    {
        m_position.z -= 4.95f;
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