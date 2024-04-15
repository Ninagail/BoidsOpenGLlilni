#include "../src/boids/boids.hpp"
#include <cstdlib>
#include "p6/p6.h"

static constexpr glm::vec3 speedMax = glm::vec3(0.02f, 0.02f, 0.02f);

void Boids::drawBoids(p6::Context& ctx) const
{
    ctx.fill = {0.3f, 0.5f, 0.9f, 1.0f};
    ctx.circle(p6::Center{this->get_position().x, this->get_position().y}, p6::Radius{0.025f});
    ctx.use_stroke = false;
}


void Boids::update_pos()
{
    m_position += m_speed;

    if (m_position.x <= -0.975f)
    {
        m_position.x += 1.95f;
    }
    else if (m_position.x >= 0.975f)
    {
        m_position.x -= 1.95f;
    }

    if (m_position.y <= -0.975f)
    {
        m_position.y += 1.95f;
    }
    else if (m_position.y >= 0.975f)
    {
        m_position.y -= 1.95f;
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