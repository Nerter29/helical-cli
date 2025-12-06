
#include <string>
#include <vector>
#include <cmath>
#include <iostream>


#include "body.h"

#define M_PI 3.14159265358979323846


Body::Body(float x, float y, float angleSpeed, std::string skin, std::string trailSkin, int maxTrailLength, std::tuple<float, float, float> color, int skinSize):
    x(x), y(y), angleSpeed(angleSpeed), skin(skin), trailSkin(trailSkin), maxTrailLength(maxTrailLength), color(color), skinSize(skinSize){}

void Body::move(float centerX, float centerY,float commonZ){
    //we move every body in a circular way, at angleSpeed speed

    float localX = x - centerX; 
    float localY = y - centerY;

    //get current angle, with trigonometry
    float angle = atan2(localY, localX);

    float nextAngle = angle + angleSpeed;

    float r = std::sqrt(localX * localX + localY * localY); // sqrt(x² + y²) : radius length (instead of x + y : bad math)

    //get the new position, thanks to trigonometry
    x = centerX + r * std::cos(nextAngle);
    y = centerY + r * std::sin(nextAngle);

    //add that point to trailList
    trailList.push_back({x,y,commonZ});

    //if the trail list is too big, we delete old trail points because we don't see them in the screen
    if(trailList.size() >= maxTrailLength * 2){
        trailList.erase(trailList.begin(), trailList.begin() + maxTrailLength);
    }

}

void Body::generateSkin(float commonZ){
    //draw an empty shpere in headList, with the center at the coordinate of the body
    headList.clear();

    float r = skinSize / 2.0f;
    float thickness = 1.0f;
    if(r != 0){
        for(float pz = commonZ - r; pz <= commonZ + r; pz++){
            for(float py = y - r; py <= y + r; py++){
                for(float px = x - r; px <= x + r; px++){

                    float dx = px - x;
                    float dy = py - y;
                    float dz = pz - commonZ;

                    float dist2 = dx*dx + dy*dy + dz*dz;
                    //we only take the points that are on the edge of the sphere
                    if(abs(dist2 - r * r) <= thickness * r){
                        headList.push_back({px, py, pz});
                    }
                }
            }
        }
    }
}