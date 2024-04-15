#include <vector>
#include "glm/vec2.hpp"
#include "p6/p6.h"

class Boids {
private:
    glm::vec3 m_position;
    glm::vec3 m_speed;
    glm::vec3 m_direction;

public:
    // attributs
    static float cohesion_force;
    static float separation_force;

    static float distance_cohesion;
    static float distance_alignment;
    static float distance_separation;

    // constructeurs
    Boids()
        : m_position(glm::vec3(0.0, 0.0, 0.0)), m_speed(glm::vec3(0.01, 0.01, 0.01)), m_direction(glm::vec3(p6::random::direction(), 0.0f)){};

    Boids(glm::vec3 position, glm::vec3 speed)
        : m_position(position), m_speed(speed){};

    ~Boids() = default;

    // Méthodes

    void drawBoids(p6::Context& ctx) const;

    void update_pos();
    void update_direction(std::vector<Boids>& boids);

    // Lois Boids

    void cohesion(const std::vector<Boids>& boids);
    void alignment(const std::vector<Boids>& boids);
    void separation(const std::vector<Boids>& boids);

    // Getters
    glm::vec3 get_position() const { return m_position; };
    glm::vec3 get_speed() const { return m_speed; }
    glm::vec3 get_direction() const { return m_direction; }

    // Setters
    void set_position();
    void set_speed();
};