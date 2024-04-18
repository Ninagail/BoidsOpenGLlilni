#include "elementdecor.hpp"
#include "proba.hpp"

ElementDecor::ElementDecor(double posX, double posY, double posZ, double s)
    : positionX(posX), positionY(posY), positionZ(posZ), size(s) {}

void ElementDecor::move(double deltaX, double deltaY, double deltaZ)
{
    positionX += deltaX;
    positionY += deltaY;
    positionZ += deltaZ;
}

void ElementDecor::setPosition(double newX, double newY, double newZ)
{
    positionX = newX;
    positionY = newY;
    positionZ = newZ;
}

void ElementDecor::setSize(double newSize)
{
    size = newSize;
}

double ElementDecor::getPositionX() const
{
    return positionX;
}

double ElementDecor::getPositionY() const
{
    return positionY;
}

double ElementDecor::getPositionZ() const
{
    return positionZ;
}

double ElementDecor::getSize() const
{
    return size;
}

void ElementDecor::applyColorVariation(double mu, double b, double time)
{
    double variation = simulateColorVariation(mu, b, time);
    // Modifier les attributs de couleur de l'élément de décor en fonction de la variation
    // Par exemple, si votre élément de décor a des attributs de couleur comme rouge, vert et bleu :
    // couleurRouge += variation;
    // couleurVerte += variation;
    // couleurBleue += variation;
}

void ElementDecor::applyWind(double location, double scale)
{
    appliquerRafaleVent(*this, location, scale);
}
