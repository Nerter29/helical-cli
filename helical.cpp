#include "body.h"
#include "helical.h"

#include <string>
#include <vector>
#include <cmath>

#define M_PI 3.14159265358979323846
#define DEPTH 200

void calculateIsomtricPosition(int& isometricX,int& isometricY, float centerX, float centerY, float centerZ,
float cameraAngle, float cameraX, float cameraY, int x, int y, int z, float cameraZoom){
    float localX = x - centerX;
    float localY = y - centerY;
    float localZ = z - centerZ;
    float rotatedX = localX * cos(cameraAngle) - localZ * sin(cameraAngle);
    float rotatedZ = localX * sin(cameraAngle) + localZ * cos(cameraAngle);
    rotatedX += centerX;
    rotatedZ += centerZ;
    isometricX = static_cast<int>(((rotatedX - rotatedZ) * cameraZoom) + cameraX);
    isometricY = static_cast<int>(((rotatedX + rotatedZ) / 2 - y) * cameraZoom + cameraY);
}


void generateScreen(int screenWidth, int screenHeight, std::string spaceMaterial, std::vector<Body>& bodies, float commonZ, std::vector<std::vector<std::string>>& screen,
float centerX, float centerY, float& cameraAngle, float cameraSpeed, bool hasToRollback, float cameraZoom){
    //clear screen
    screen.assign(screenHeight, std::vector<std::string>(screenWidth, spaceMaterial));
    cameraAngle += cameraSpeed;
    if(cameraAngle > 2  * M_PI){cameraAngle -= 2 * M_PI;}

    //follow center
    float baseIsometricX = (centerX - commonZ) * cameraZoom;
    float baseIsometricY = ((centerX + commonZ) / 2 - centerY) * cameraZoom;
    float cameraX = screenWidth  / 2.0f - baseIsometricX;
    float cameraY = screenHeight / 2.0f - baseIsometricY;

    for(int i = 0; i < bodies.size(); i++){
        Body& body = bodies[i];
        for(int j = body.trailList.size() - 1; j >= 0 ; j--){
            std::vector<float>& trailUnit = body.trailList[j];
            int x = trailUnit[0];
            int y = trailUnit[1];
            int z = trailUnit[2];

            //rollback everything when we go to deep in space, so commonZ doesn't grow iniftly
            if(hasToRollback){
                trailUnit[2] -= DEPTH;
                z -= DEPTH;
            }

            //isometric view
            int isometricX, isometricY;
            calculateIsomtricPosition(isometricX, isometricY, centerX, centerY, commonZ, cameraAngle, cameraX, cameraY, x, y, z, cameraZoom);

            if(isometricX >= 0 && isometricX < screenWidth && isometricY >= 0 && isometricY < screenHeight){
                if(j == body.trailList.size() - 1){ //display body
                    screen[isometricY][isometricX] = getColoredString(body.color, body.skin);
                }
                else{ //display body's trail
                    screen[isometricY][isometricX] = getColoredString(body.color, body.trailSkin);
                }     
            }   
        }
    }
}
std::string getColoredString(std::tuple<float, float, float> color, std::string skin){
    //get the colored string corresponding to the ratio of index / maxIndex, on a color gradient

    int r = std::get<0>(color);
    int g = std::get<1>(color);
    int b = std::get<2>(color);

    return "\033[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m" + skin + "\033[0m";
}

void spawnBodies(std::vector<Body>& bodies, std::vector<std::vector<float>> bodiesAttributes, int maxTrailLength){

    for(int i = 0; i < bodiesAttributes.size(); i++){
        float x = bodiesAttributes[i][0];
        float y = bodiesAttributes[i][1];
        float angleSpeed = bodiesAttributes[i][2];
        float r = bodiesAttributes[i][3];
        float g = bodiesAttributes[i][4];
        float b = bodiesAttributes[i][5];
        std::tuple<float, float, float> color = {r, g, b};
        Body body = Body(x, y, angleSpeed, "@", "*", maxTrailLength, color);
        bodies.push_back(body);
    }
}

