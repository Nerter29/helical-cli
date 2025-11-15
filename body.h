#ifndef ROCKET_H
#define ROCKET_H

#include <string>
#include <vector>
#include <cmath>

class Body { //spacial body : "astre"
public:
    float x;
    float y;
    float angleSpeed;
    std::vector<std::vector<float>> trailList; // trailList = {{x, y, z}, {x, y, z}}
    char skin;
    char trailSkin;

    Body(float x, float y, float angleSpeed, char skin, char trailSkin);

    void move(float centerX, float centerY,float commonZ);
    
};

#endif // ROCKET_H
