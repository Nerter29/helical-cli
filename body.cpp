
#include <string>
#include <vector>
#include <cmath>
#include <iostream>


#include "body.h"

#define M_PI 3.14159265358979323846


Body::Body(float x, float y, float angleSpeed, char skin, char trailSkin):
    x(x), y(y), angleSpeed(angleSpeed), skin(skin), trailSkin(trailSkin){}

void Body::move(float centerX, float centerY,float commonZ){
    float offsetX = x - centerX; 
    float offsetY = y - centerY;

    float theta = atan2(offsetY, offsetX);
    float nextTheta = theta + angleSpeed;

    float r = std::sqrt(offsetX * offsetX + offsetY * offsetY);
    x = centerX + r * std::cos(nextTheta);
    y = centerY + r * std::sin(nextTheta);


    trailList.push_back({x,y,commonZ});
    std::cout << x << " " << y << " " << theta << " " << r << " "<< angleSpeed << "\n";

}