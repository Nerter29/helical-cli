#ifndef BODY_H
#define BODY_H

#include <string>
#include <vector>
#include <cmath>

class Body { //spacial body : "astre"
public:
    float x;
    float y;
    float angleSpeed;
    std::vector<std::vector<float>> trailList; // trailList = {{x, y, z}, {x, y, z}}
    std::vector<std::vector<float>> headList; // headList = {{x, y, z}, {x, y, z}}

    std::string skin;
    std::string trailSkin;
    int maxTrailLength;
    std::tuple<float, float, float> color;
    int skinSize;

    Body(float x, float y, float angleSpeed, std::string skin, std::string trailSkin, int maxTrailLength, std::tuple<float, float, float> color, int skinSize);

    void move(float centerX, float centerY,float commonZ);
    void generateSkin(float commonZ);
    
};

#endif // BODY_H
