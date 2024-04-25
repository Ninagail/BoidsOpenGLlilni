#pragma once

#include <glm/glm.hpp>
#include "../src/model/model.hpp"
#include "../src/person/person.hpp"

class Camera {
private:
    Person& m_target;   // Référence vers le personnage à suivre
    Model&  m_model;    // Référence vers le modèle de la caméra
    float   m_distance; // Distance entre la caméra et le personnage
    float   m_height;   // Hauteur de la caméra par rapport au personnage
    float   m_Pitch;    // Angle de hauteur de la caméra
    float   m_Yaw;      // Angle de rotation horizontale de la caméra

public:
    // Constructeur prenant une référence vers le personnage et des valeurs initiales pour la distance, le pitch et le yaw
    Camera(Person& target, Model& model, float distance = 3.0f, float pitch = 50.0f, float yaw = 20.0f)
        : m_target(target), m_model(model), m_distance(0.1f), m_height(-.5f), m_Pitch(pitch), m_Yaw(yaw) {}

    glm::vec3 getPosition() const
    {
        // Calculer et renvoyer la position de la caméra en fonction de ses autres paramètres
        // Par exemple, vous pouvez simplement retourner la position cible du personnage
        return m_target.getPosition();
    }

    // Fonction pour mettre à jour la vue de la caméra en fonction de la position et de l'orientation du personnage
    void getViewMatrix(glm::mat4& ViewMatrix)
    {
        glm::mat4 modelRotationMatrix = glm::rotate(glm::mat4(45.0f), m_target.getRotationAngle(), glm::vec3(0.0f, 1.0f, 0.0f));
        // Calcul de la position de la caméra en fonction de la viewMatrix du jouesur
        glm::mat4 invPlayerViewMatrix = glm::inverse(ViewMatrix);
        auto      playerPos           = glm::vec3(invPlayerViewMatrix[3]);

        glm::vec3 cameraPos = playerPos + m_distance * glm::vec3(modelRotationMatrix * glm::vec4(invPlayerViewMatrix[2])) + m_height * glm::vec3(invPlayerViewMatrix[1]);
        // Mise à jour de la vue de la caméra
        ViewMatrix = glm::lookAt(cameraPos, playerPos, glm::vec3(0.0f, 1.0f, 0.0f));
        m_model.setRotation(m_target.getRotationAngle());
    };
    // Fonctions pour ajuster les angles de pitch et yaw de la caméra
    void adjustPitch(float amount)
    {
        m_Pitch += amount;
    }
    void adjustYaw(float amount)
    {
        m_Yaw += amount;
    }
};