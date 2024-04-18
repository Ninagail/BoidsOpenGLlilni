#pragma once

#include <glm/glm.hpp>
#include "../src/scene/loadShader.hpp"
#include "glm/fwd.hpp"
#include "p6/p6.h"

#pragma once

#include "../src-common/glimac/FreeflyCamera.hpp"

class Camera {
private:
    Freefly m_Freefly; // Instance de la classe Freefly

public:
    // Constructeur par défaut
    Camera()
        : m_Freefly() {}

    // Méthodes de déplacement et de rotation
    void moveLeft(float t) { m_Freefly.moveLeft(t); }
    void moveFront(float t) { m_Freefly.moveFront(t); }
    void rotateLeft(float degrees) { m_Freefly.rotateLeft(degrees); }
    void rotateUp(float degrees) { m_Freefly.rotateUp(degrees); }

    // Obtenir la matrice de vue
    glm::mat4 getViewMatrix() const { return m_Freefly.getViewMatrix(); }

    // Gestion des événements clavier et souris
    void handleKeyPressed(p6::Key key)
    {
        if (key.logical == "d")
            moveLeft(-0.1f);
        if (key.logical == "q")
            moveLeft(0.1f);
        if (key.logical == "z")
            moveFront(0.1f);
        if (key.logical == "s")
            moveFront(-0.1f);
    }

    void handleKeyReleased(p6::Key key) {}

    void handleMouseDragged(const p6::MouseDrag& button)
    {
        rotateLeft(button.delta.x * 5);
        rotateUp(-button.delta.y * 5);
    }

    void handleMouseScrolled(const p6::MouseScroll& scroll)
    {
        moveFront(-scroll.dy);
    }
};
