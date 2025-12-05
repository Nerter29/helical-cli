
#include <string>
#include <vector>
#include <cmath>
#include <iostream>


#include "body.h"

#define M_PI 3.14159265358979323846


Body::Body(float x, float y, float angleSpeed, std::string skin, std::string trailSkin, int maxTrailLength, std::tuple<float, float, float> color, int skinSize):
    x(x), y(y), angleSpeed(angleSpeed), skin(skin), trailSkin(trailSkin), maxTrailLength(maxTrailLength), color(color), skinSize(skinSize){}

void Body::move(float centerX, float centerY,float commonZ){
    float offsetX = x - centerX; 
    float offsetY = y - centerY;

    float theta = atan2(offsetY, offsetX);
    float nextTheta = theta + angleSpeed;

    float r = std::sqrt(offsetX * offsetX + offsetY * offsetY);
    x = centerX + r * std::cos(nextTheta);
    y = centerY + r * std::sin(nextTheta);


    trailList.push_back({x,y,commonZ});
    if(trailList.size() >= maxTrailLength * 2){
        trailList.erase(trailList.begin(), trailList.begin() + maxTrailLength);
    }
    //std::cout << x << " " << y << " " << theta << " " << r << " "<< angleSpeed << "\n";

}

void Body::generateSkin(float centerX, float centerY,float commonZ){
    headList.clear();

    float r = skinSize / 2.0f;      // rayon
    float thick = 1.0f;             // épaisseur du contour
    float r2 = r * r;

    for(float pz = commonZ - r; pz <= commonZ + r; pz++){
        for(float py = centerY - r; py <= centerY + r; py++){
            for(float px = centerX - r; px <= centerX + r; px++){

                float dx = px - centerX;
                float dy = py - centerY;
                float dz = pz - commonZ;

                float dist2 = dx*dx + dy*dy + dz*dz;

                if(abs(dist2 - r2) <= thick * r){
                    headList.push_back({px, py, pz});  // x,y,z renommés → px,py,pz
                }
            }
        }
    }

}