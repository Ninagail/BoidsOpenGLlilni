#ifndef ELEMENTDECOR_H
#define ELEMENTDECOR_H

class ElementDecor {
private:
    double positionX;
    double positionY;
    double positionZ;
    double size;

public:
    ElementDecor(double posX, double posY, double posZ, double s);
    void   move(double deltaX, double deltaY, double deltaZ);
    void   setPosition(double newX, double newY, double newZ);
    void   setSize(double newSize);
    double getPositionX() const;
    double getPositionY() const;
    double getPositionZ() const;
    double getSize() const;
    void   applyColorVariation(double mu, double b, double time);
    void   applyWind(double location, double scale);
};

#endif
